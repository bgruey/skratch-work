import csv
import logging
import time
import pwd

import boto3
import os

from io import BytesIO
from pyspark import SparkContext, SparkConf
#from pyspark.sql import SparkSession

spark_master = os.getenv("SPARK_MASTER")

#spark = SparkSession.builder.master(spark_master).appName("temp-test").getOrCreate()

conf = SparkConf().setAppName("temp-test").setMaster(spark_master)
spark_context = SparkContext(conf=conf)


logging.error("\n\nUsername: %s\n\n---------", pwd.getpwuid(os.getuid()).pw_name)

client = boto3.client("s3")

r = client.get_object(
    Bucket="raw",
    Key="data-1.csv"
)

data = r['Body'].read()
print(data)
client.put_object(
    Bucket="clean",
    Key="cleaned-1.csv",
    Body=data
)
print("Job Success!")
