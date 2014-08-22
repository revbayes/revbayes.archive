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

def get_best_n(d,idx,n=5,p='posterior'):
    if idx < 0:
        print('ERROR: idx < 0')
    if idx > len(d):
        print('ERROR: idx > len(d)')
    if p not in ['posterior','prior','likelihood']:
        print('WARNING: p=\'' + p + '\' invalid, set p=\'posterior\'')
        p = 'posterior'
    K = d[idx][p]
    best = sorted(range(len(K)), key=lambda x: K[x])[-n:][::-1]
    ret = {}
    for k in d[idx].keys():
        ret[k] = [] 
    for i in best:
        for k in d[idx].keys():
            ret[k].append(d[idx][k][i])
    return(ret)
