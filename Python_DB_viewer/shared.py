
def find_max(collection):
	if not collection:
		return (None, None)
	max_idx = list(collection.keys())[0] if type(collection) is dict else 0
	max_value = collection[max_idx]
	for key in collection:
		if collection[key] > collection[max_idx]:
			max_idx = key
			max_value = collection[key]

	return (max_idx, max_value)


def find_diffrenet(previous, current):
	if previous and current:
		for i, x in enumerate(previous):
			if x != current[i]:
				return i

	return -1


def sort_by_y(x, y):
	if len(x) != len(y):
		raise Exception("x and y must be correlated and from the same length")
	ln = len(y)
	for i in range(ln):
		for j in range(i + 1, ln):
			if y[j] > y[i]:
				tx = x[i]
				ty = y[i]

				x[i] = x[j]
				y[i] = y[j]

				x[j] = tx
				y[j] = ty
	return x, y


def sort_dict(statistics):
	return dict(sorted(statistics.items(), key=lambda kv: (kv[1], kv[0]), reverse=True))


def date_sort(lst):
	lst_length = len(lst)
	for i in range(lst_length):
		for j in range(i + 1, lst_length):
			if lst[j].date < lst[i].date:
				temp = lst[j]
				lst[j] = lst[i]
				lst[i] = temp
	return lst
