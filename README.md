# news-video-emotion-analysis
A small collection of code for performing emotion analysis on news videos

Simple analysis commands:

Install `pandas`

Extract `data.tar.gz` to `data`

`python get_mean.py cnn`

`python count_scenes.py fox`

Video commands:

Install `pandas`

Extract `data.tar.gz` to `data`

Install `youtube-dl`

`./download_youtube_videos.sh` (This will take a long time and consume around 40GB)

`python extract_scenes.py cnn > scenes.txt`

`./make_gifs_from_scenes.sh cnn < scenes.txt`

## Problems

Let me know if there are any problems by contacting me or raising an issue.  The instructions are not thouroughly tested.
