import csv
import logging
import time

import minio
import os

from io import BytesIO
from pyspark import SparkContext, SparkConf
#from pyspark.sql import SparkSession

spark_master = os.getenv("SPARK_MASTER")

#spark = SparkSession.builder.master(spark_master).appName("temp-test").getOrCreate()

conf = SparkConf().setAppName("temp-test").setMaster(spark_master)
spark_context = SparkContext(conf=conf)


client = minio.Minio(
    access_key="admin",
    secret_key="adminadmin",
    endpoint="minio-s3:9000",
    secure=False
)

r = client.get_object(
    bucket_name="raw",
    object_name="data-1.csv"
)

print(r.data)
client.put_object(
    bucket_name="clean",
    object_name="cleaned-1.csv",
    data=BytesIO(r.data),
    length=len(r.data)
)
print("Job Success!")
