# SparkSQL with DBT

SparkSQL overloads the dataframe to make it accessible for SQL querying.
https://spark.apache.org/examples.html

The dbt spark connection uses thrift.
Setup for dbt form s3 around here: https://www.youtube.com/watch?v=B1zEKtoD8QY
around minute 18 or so is the setup

About halfway down: https://medium.com/data-engineering-on-cloud/dbt-models-using-spark-on-emr-4d6c7ea478e9

So basically I think we create a model to lead the s3 data as a view, then another model to
transform it into the intermediate bucket and save it as materialized. After loading that
cleaned data into the new one, do we delete the s3 data?