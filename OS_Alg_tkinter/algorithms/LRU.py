# LRU page replacement algorithm implementation in python
# Created By: Suman Mohammad-HGH
from ui.Table import Table


class LRU:
    def __init__(self, capacity, inputlist, *otherinputs):
        self.capacity = capacity
        self.inputlist = inputlist
        self.otherinputs = otherinputs

    def render_lru(self):
        f, st, fault, pf = self.otherinputs
        print("\nString|Frame →\t", end='')
        for i in range(self.capacity):
            print(i, end=' ')
        print("Fault\n   ↓\n")
        for i in self.inputlist:
            if i not in f:
                if len(f) < self.capacity:
                    f.append(i)
                    st.append(len(f) - 1)
                else:
                    ind = st.pop(0)
                    f[ind] = i
                    st.append(ind)
                pf = 'Yes'
                fault += 1
            else:
                st.append(st.pop(st.index(f.index(i))))
                pf = 'No'
            print("   %d\t\t" % i, end='')
            for x in f:
                print(x, end=' ')
            for x in range(self.capacity - len(f)):
                print(' ', end=' ')
            print(" %s" % pf)

        self.divert_to_table([
            ('Reference:', self.inputlist),
            ('Total Requests:', len(self.inputlist)),
            ('Total Page Faults:', fault),
            ('Fault Rate:', (fault / len(self.inputlist)) * 100),
        ])

    def divert_to_table(self, lst):
        tableObj = Table(lst)
        tableObj.render_table()
