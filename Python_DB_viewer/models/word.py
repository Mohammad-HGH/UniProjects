from shared import find_max, sort_dict, date_sort

class Comment:
	S = []
	def __init__(self, text, post_id, owner, likes, score, date) -> None:
		self._ = text
		self.likes = likes
		self.score = score
		self.owner = owner
		self.date = date
		self.post_id = post_id
		Comment.S.append(self)

	@staticmethod
	def average():
		sum = 0
		sum_likes = 0
		for x in Comment.S:
			sum_likes += x.likes
			sum += x.score * x.likes
		return sum / sum_likes

class Word:
	S = []
	def __init__(self, id, post_id, word, owner, date, likes=0, score=0.0):
		self.id = id
		self.post_id = post_id
		self._ = word.lower()
		self.owner = owner
		self.date = date
		Word.S.append(self)


	@staticmethod
	def iterations(mark = '#', date = None, owner = None, post_id = None):
		# write additional code for filtering conditions
		# if you want to analys all the words => mark=''
		iters = dict()
		for word in Word.S:
			if word._[0] == mark:
				actual_word = word._[1:].capitalize()
				iters[actual_word] = iters[actual_word] + 1 if actual_word in iters else 1

		return sort_dict(iters)


	@staticmethod
	def most_used(limit = None):
		word_stats = Word.iterations()
		if not limit or limit <= 0:
			return word_stats
		temp = word_stats.copy()
		filtered_keys = []
		for i in range(limit):
			k, v = find_max(temp)
			if not k and not v:
				break
			filtered_keys.append(k)
			del temp[k] # remove most used to find the limit floor

		if not temp:
			return word_stats

		words = list(filter(lambda idx: idx in filtered_keys, word_stats))
		return (words, list(map(lambda word: word_stats[word], words)))


	def __str__(self) -> str:
		return self._.capitalize()

	@staticmethod
	def timeline(word, mark = '#'):
		# if you want to analys all the words => mark=''
		# find specific word's timeline
		sames = list(filter(lambda w: w._.lower() == f'{mark}{word}'.lower(), Word.S))
		sames = date_sort(sames)
		tl = {}
		for sw in sames:
			tl[sw.date] = tl[sw.date] + 1 if sw.date in tl else 1
		return tl

