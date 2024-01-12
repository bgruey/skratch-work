import boto3

client = boto3.client("s3")

r = client.get_object(
    Bucket="raw",
    Key="data-1.csv"
)

print(r['Body'].read())
