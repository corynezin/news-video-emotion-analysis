import sys
import time
import datetime
import itertools
from collections import Counter
import pandas as pd

tag = sys.argv[1]
threshold_start = float(sys.argv[2])
threshold_end = float(sys.argv[3])

emotions = ['joy', 'fear', 'disgust', 'sadness', 'anger',
            'surprise', 'contempt', 'valence', 'engagement']
names = ['tag', 'videoId', 'frame', 'timestamp', 'faceId'] + emotions
df = pd.read_csv(f'data/{tag}.csv', names=names)

emotion = 'anger'
times = {} # map (videoId, start) -> end
videoIds = df.videoId.unique()
thresh = df[df[emotion] > threshold_start]
thresh = thresh[thresh[emotion] < threshold_end]

gb = thresh.groupby('videoId')
total_seconds = df.groupby('videoId').max()['timestamp'].sum()
total_hours = total_seconds / 3600

timethresh = 5
keys = gb.groups.keys()
for key in keys:
    group = gb.get_group(key)
    lasttime = group.iloc[0]['timestamp']
    start = lasttime
    times[(key, start)] = lasttime
    n = 0
    while n < len(group):
        time = group.iloc[n]['timestamp']
        if time - lasttime < timethresh:
            lasttime = time
            times[(key, start)] = time
        else:
            start = time
            lasttime = time
            times[(key, start)] = start
        n += 1

times = {key: val for key, val in times.items() if val - key[1] >= 1}

for key, val in times.items():
    print(key[0], key[1], val-key[1])
