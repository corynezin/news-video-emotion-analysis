import sys
import pandas as pd

tag = sys.argv[1]

emotions = ['joy', 'fear', 'disgust', 'sadness', 'anger',
                    'surprise', 'contempt', 'valence', 'engagement']

names = ['tag', 'videoId', 'frame', 'timestamp', 'faceId'] + emotions

df = pd.read_csv(f'data/{tag}.csv', names=names)
maximums = df.groupby(['videoId']).max()
indices = df.groupby(['videoId']).idxmax()
print('Mean Anger:', df['anger'].mean())
