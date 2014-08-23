import re

def read_events(fn='/Users/mlandis/data/bayarea/output/bg_2rate.events.txt'):
    f = open(fn,'r')
    lines = f.readlines()
    f.close()
    return(lines)

def get_events(lines):
    events = {}
    for l in lines:
        y = l.split('\t')
        x = re.findall('\\[([^]]+)\\]', y[-1])
        n = len(x)
        for i in range(1,n):
            s = x[i][1:-1]
            toks = s.split(';')
            taxon_index = int(toks[0].split('=')[1])
            if not events.has_key( taxon_index ):
                events[ taxon_index ] = {}
                events[ taxon_index ]['iteration'] = []
                events[ taxon_index ]['posterior'] = []
                events[ taxon_index ]['likelihood'] = []
                events[ taxon_index ]['prior'] = []
            events[taxon_index]['iteration'].append(int(y[0]))
            events[taxon_index]['posterior'].append(float(y[1]))
            events[taxon_index]['likelihood'].append(float(y[2]))
            events[taxon_index]['prior'].append(float(y[3]))
            for j in range(1,len(toks)):
                k,v = toks[j].split('=')
                if not events[ taxon_index ].has_key( k ):
                    events[ taxon_index ][ k ] = []
                if k == 'nd' or k == 'ch0' or k == 'ch1':
                    v = [ int(b) for b in v ]
                elif k == 'cs':
                    v = v
                elif k == 'bn':
                    v = int(v)
                elif k == 'ev':
                    v = re.sub( '[{|}]', '', v[1:-1] ).split(',')
                    tmp = []
                    for vi in range(len(v)/4):
                        time=float(v[vi*4+0][2:])
                        age=float(v[vi*4+1][2:])
                        state=int(v[vi*4+2][2:])
                        idx=int(v[vi*4+3][2:])
                        tmp.append( {"time":time,"age":age,"state":state,"idx":idx} )
                    v = tmp

                events[ taxon_index ][ k ].append(v)
    return events

def get_best(d,idx,n=10,f=None,p='posterior'):
    if idx < 0:
        print('ERROR: idx < 0')
    if idx > len(d):
        print('ERROR: idx > len(d)')
    if p not in ['posterior','prior','likelihood']:
        print('WARNING: p=\'' + p + '\' invalid, set p=\'posterior\'')
        p = 'posterior'
    K = d[idx][p]
    if n < 0:
        print('ERROR: n < 0')
    if f is not None:
        if f > 1.:
            f = 1.
        if f < 0.:
            print('ERROR: f < 0.')
        n = f * len(K)
    best = sorted(range(len(K)), key=lambda x: K[x])[-n:][::-1]
    ret = {}
    for k in d[idx].keys():
        ret[k] = []
    for i in best:
        for k in d[idx].keys():
            ret[k].append(d[idx][k][i])
    return(ret)

def get_gain_loss(d, freqs=True):
    num_char = len(d['nd'][0])
    v = []
    for i in range(2):
        v.append([0.]*num_char)
    n = 0
    for ev in d['ev']:
        n += 1
        if len(ev) is 0:
            next
        else:
            for e in ev:
                v[e['state']][e['idx']] += 1.
    if freqs:
        for i in range(2):
            for j in range(num_char):
                v[i][j] = v[i][j] / n
    return(v)


def get_area_pair(d,freqs=True,k='nd'):
    
    num_char = len(d[k][0])
    v = []
    for i in range(num_char):
        v.append([0.]*num_char)
   
    if not d.has_key(k) or k not in ['nd','ch0','ch1']:
        print('ERROR: \'' + k + '\' invalid key')
        return

    n = 0
    for x in d[k]:
        n += 1
        for i in range(num_char):
            for j in range(i,num_char):
                if x[i] is 1 and x[j] is 1:
                    v[i][j] += 1.
                    v[j][i] = v[i][j]
    if freqs:
        for i in range(num_char):
            for j in range(num_char):
                v[i][j] = v[i][j] / n
    return(v)

def get_clado_state(d,freqs=True,includeNarrow=True):

    if not d.has_key('ch0') or not d.has_key('ch1'):
        print('ERROR: no cladogenic state recorded')
        return

    num_char = len(d['nd'][0])
    v = {'s':0.,'a':0.,'p':0.}
   
    n = 0
    for i,x in enumerate(d['cs']):
        if includeNarrow:
            n += 1
            v[x] += 1
        elif sum(d['nd'][i]) > 1:
            n += 1
            v[x] += 1


    for k in v.keys():
        v[k] = v[k]/n

    return(v)


# not quite right...
# 1) prob allopatry splits two areas given a certain age
def get_allopatry_prob(d,freqs=True):
    
    if not d.has_key('ch0') or not d.has_key('ch1'):
        print('ERROR: no cladogenic state recorded')
        return

    num_char = len(d['nd'][0])

    v = {}
    n = 0
    for k in range(len(d['nd'])):
        if d['cs'][k] == 'a':
            nd_s = "".join([str(x) for x in d['nd']])
            if not v.has_key(nd_s):
                v[nd_s] = {}
            for y,z in zip(d['ch0'],d['ch1']):
                print(y,z,d['nd'][k])
                n += 1
                ch0_s = "".join([str(x) for x in y])
                ch1_s = "".join([str(x) for x in z])
                y_tmp = y
                if ch0_s > ch1_s:
                    y_tmp = [ a*2 for a in y_tmp ]
                b = [ sum(a) for a in zip(y_tmp,z) ]
                #print b

            if freqs:
                for i in range(num_char):
                    for j in range(num_char):
                        v[i][j] = v[i][j] / n

    return(v)
