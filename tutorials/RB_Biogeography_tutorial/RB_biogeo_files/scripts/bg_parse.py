import re

def read_events(fn):
    f = open(fn,'r')
    lines = f.readlines()
    f.close()
    return(lines)

def get_events(fn='../data/bg_2rate.events.txt'):
    lines=read_events(fn)
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

def get_best(d,n=5,f=None,p='posterior'):
    if p not in ['posterior','prior','likelihood']:
        print('WARNING: p=\'' + p + '\' invalid, set p=\'posterior\'')
        p = 'posterior'
    K = d[p]
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
    for k in d.keys():
        ret[k] = []
    for i in best:
        for k in d.keys():
            ret[k].append(d[k][i])
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
    if freqs and n == 0:
        return([0.]*num_char)
    if freqs:
        for i in range(2):
            for j in range(num_char):
                v[i][j] = v[i][j] / n
    return(v)


def get_area_pair(d,k='nd',freqs=True):
    
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
    if freqs and n == 0:
        return([0.]*num_char)
    if freqs:
        for i in range(num_char):
            for j in range(num_char):
                v[i][j] = v[i][j] / n
    return(v)

def get_clado_state(d,minSize=1,freqs=True):

    if not d.has_key('ch0') or not d.has_key('ch1'):
        print('ERROR: no cladogenic state recorded')
        return

    num_char = len(d['nd'][0])
    v = {'s':0.,'a':0.,'p':0.,'w':0.}
   
    n = 0
    for i,x in enumerate(d['cs']):
        if sum(d['nd'][i]) >= minSize:
            n += 1
            v[x] += 1

    if freqs:
        if n == 0:
            return([0.]*num_char)
        for k in v.keys():
            v[k] = v[k]/n

    return(v)


def get_clado_prob(d,freqs=True):
    
    if not d.has_key('ch0') or not d.has_key('ch1'):
        print('ERROR: no cladogenic state recorded')
        return

    num_char = len(d['nd'][0])

    v = {}
    n = 0
    for k in range(len(d['nd'])):
        if d['cs'][k] == 'a':
            n += 1
            nd_s = "".join([str(x) for x in d['nd'][k]])
            if not v.has_key(nd_s):
                v[nd_s] = {}
            y = d['ch0'][k]
            z = d['ch1'][k]
            print(y,z,d['nd'][k])
            ch0_s = "".join([str(x) for x in y])
            ch1_s = "".join([str(x) for x in z])
            y_tmp = y
            z_tmp = z
            if ch0_s < ch1_s:
                y_tmp = [ a*2 for a in y_tmp ]
            else:
                z_tmp = [ a*2 for a in z_tmp ]
            b = [ sum(a) for a in zip(y_tmp,z_tmp) ]
            b_s = "".join([str(x) for x in b])
            if not v[nd_s].has_key(b_s):
                v[nd_s][b_s] = 0.
            v[nd_s][b_s] += 1.

    if freqs and n == 0:
        return([0.]*num_char)
    if freqs:
        for ki in v.keys() :
            for kj in v[ki].keys():
                v[ki][kj] = v[ki][kj] / n

    return(v)
