#!/bin/bash

echo "Setting up s3 files"
python3 ./setup/load_s3_data.py

/opt/spark/bin/spark-submit \
    --master spark://spark-brain:7077 \
    --name clean-job ./jobs/clean.py


cd local 

dbt debug --config-dir --profiles-dir=./profiles

dbt run \
    --profiles-dir=./profiles \
    --project-dir=./ \
    --target=local
