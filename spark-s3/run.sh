dexecw() {
  docker \
    exec $(docker ps | grep spark-worker | awk '{print $1}') \
      /opt/spark/bin/spark-submit \
        --master spark://localhost:7077 \
        --name clean-job \
        ./jobs/clean.py
}

echo "Setting up s3 files"
python3 ./setup/load_s3_data.py

dexecw