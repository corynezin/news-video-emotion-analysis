n=0
tag=$1
while read line
do
  num=$(echo "$line" | cut -d' ' -f1)
  start=$(echo "$line" | cut -d' ' -f2)
  time=$(echo "$line" | cut -d' ' -f3)
  echo $line
  echo $start
  echo $time
  ffmpeg -nostdin -ss $start -t $time -i ${tag}-videos/$num.mp4 -filter:v scale=720:-1 -f gif gifs/$n.gif
  n=$((n+1))
done < /dev/stdin
