# two version beam search

# from numpy import array


# #main function
# #beta here is width of beam and distances can be considered as weights.
# def beam_search(distances, beta):
#     #initialising some record
#     paths_so_far = [[list(), 0]]

#     #traverse through the neighbouring vertices row by row.
#     for idx, tier in enumerate(distances):
#         if idx > 0:
#             print(f'Paths kept after tier {idx-1}:')
#             print(*paths_so_far, sep='\n')
#         paths_at_tier = list()

#         for i in range(len(paths_so_far)):
#             path, distance = paths_so_far[i]

#             # Extending the paths
#             for j in range(len(tier)):
#                 path_extended = [path + [j], distance + tier[j]]
#                 paths_at_tier.append(path_extended)

#         paths_ordered = sorted(paths_at_tier, key=lambda element: element[1])

#         # The best paths are saved
#         paths_so_far = paths_ordered[:beta]
#         print(f'\nPaths reduced to after tier {idx}: ')
#         print(*paths_ordered[beta:], sep='\n')

#     return paths_so_far


# #Distance matrix
# dists = [[1, 4, 6, 8],
#          [5, 2, 3, 4]]
# dists = array(dists)

# # Calculating the best paths
# best_paths = beam_search(dists, 2)
# print('\nThe best paths:')
# for beta_path in best_paths:
#     print(beta_path)

# ------------------------------------------------------------------------------------ #

from __future__ import unicode_literals, print_function
from __future__ import absolute_import
from __future__ import division

import tensorflow as tf

tf.app.flags.DEFINE_integer(
    'beam_size', 4, 'beam size for beam search decoding.')
tf.app.flags.DEFINE_integer('vocab_size', 40, 'vocabulary size.')
tf.app.flags.DEFINE_integer('batch_size', 5, 'the batch size.')
tf.app.flags.DEFINE_integer('num_steps', 10, 'the batch size.')
tf.app.flags.DEFINE_integer('embedding_size', 50, 'the batch size.')

FLAGS = tf.app.flags.FLAGS


with tf.Graph().as_default():
    batch_size = FLAGS.batch_size
    beam_size = FLAGS.beam_size  # Number of hypotheses in beam
    vocab_size = FLAGS.vocab_size  # Output vocabulary size
    num_steps = FLAGS.num_steps
    embedding_size = FLAGS.embedding_size
    embedding = tf.random_normal(
        [vocab_size, embedding_size], -2, 4, dtype=tf.float32, seed=0)
    output_projection = [
        tf.random_normal([embedding_size, vocab_size], mean=2,
                            stddev=1, dtype=tf.float32, seed=0),
        tf.random_normal([vocab_size], mean=0, stddev=1,
                            dtype=tf.float32, seed=0),
    ]
    index_base = tf.reshape(
        tf.tile(tf.expand_dims(tf.range(batch_size) * beam_size, axis=1), [1, beam_size]), [-1])

    log_beam_probs, beam_symbols = [], []

    def beam_search(prev, i):
        if output_projection is not None:
            prev = tf.nn.xw_plus_b(
                prev, output_projection[0], output_projection[1])
            # (batch_size*beam_size, embedding_size) -> (batch_size*beam_size, vocab_size)

        log_probs = tf.nn.log_softmax(prev)

        if i > 1:
            # total probability
            log_probs = tf.reshape(tf.reduce_sum(tf.stack(log_beam_probs, axis=1), axis=1) + log_probs,
                                   [-1, beam_size * vocab_size])
            # (batch_size*beam_size, vocab_size) -> (batch_size, beam_size*vocab_size)

        best_probs, indices = tf.nn.top_k(log_probs, beam_size)
        # (batch_size, beam_size)
        indices = tf.squeeze(tf.reshape(indices, [-1, 1]))
        best_probs = tf.reshape(best_probs, [-1, 1])
        # (batch_size*beam_size)

        symbols = indices % vocab_size       # which word in vocabulary
        beam_parent = indices // vocab_size  # which hypothesis it came from

        beam_symbols.append(symbols)

        # (batch_size*beam_size, num_steps)
        real_path = beam_parent + index_base
        # get rid of the previous probability
        if i > 1:
            pre_sum = tf.reduce_sum(tf.stack(log_beam_probs, axis=1), axis=1)
            pre_sum = tf.gather(pre_sum, real_path)
        else:
            pre_sum = 0
        log_beam_probs.append(best_probs-pre_sum)
        # adapt the previous symbols according to the current symbol
        if i > 1:
            for j in range(i)[:0:-1]:
                beam_symbols[j-1] = tf.gather(beam_symbols[j-1], real_path)
                log_beam_probs[j-1] = tf.gather(log_beam_probs[j-1], real_path)

        return tf.nn.embedding_lookup(embedding, symbols)
        # (batch_size*beam_size, embedding_size)

    # Setting up graph.
    init_input = tf.placeholder(tf.float32, shape=[batch_size, embedding_size])
    next_input = init_input

    for i in range(num_steps):
        next_input = beam_search(next_input, i+1)

    seq_rank = tf.stack(values=beam_symbols, axis=1)
    seq_rank = tf.reshape(seq_rank, [batch_size, beam_size, num_steps])
    # (batch_size*beam_size, num_steps)

    init_in = tf.random_uniform(
        [batch_size], minval=0, maxval=vocab_size, dtype=tf.int32, seed=0),
    init_emb = tf.squeeze(tf.nn.embedding_lookup(embedding, init_in))
    session = tf.InteractiveSession()
    init_emb = init_emb.eval()

    seq_rank = session.run(seq_rank, feed_dict={init_input: init_emb})
    best_seq = seq_rank[:, 1, :]
    for i in range(batch_size):
        print("rank %s" % i, end=": ")
        print(best_seq[i])
