n=0
while read url; do
  echo $n
  if [ ! -f fox-videos/$n.mp4 ]; then
    timeout 120s youtube-dl $url -o fox-videos/$n --recode-video mp4 --postprocessor-args "-strict experimental"
  fi
  n=$((n+1))
done < data/fox.txt

n=0
while read url; do
  echo $n
  if [ ! -f cnn-videos/$n.mp4 ]; then
    timeout 120s youtube-dl $url -o cnn-videos/$n --recode-video mp4 --postprocessor-args "-strict experimental"

  fi
  n=$((n+1))
done < data/cnn.txt

while [ 1 ]; do fg 2> /dev/null; [ $? == 1 ] && break; done
