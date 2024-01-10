import minio

from io import BytesIO

client = minio.Minio(
    access_key="admin",
    secret_key="adminadmin",
    endpoint="minio-s3:9000",
    secure=False
)


def clear_bucket(bucket_name: str):
    print("Clearing bucket ", bucket_name)
    if client.bucket_exists(bucket_name=bucket_name):
        print("\tDeleting bucket ", bucket_name)
        for name in client.list_objects(bucket_name=bucket_name):
            print("\t\tDeleting object ", name.object_name)
            client.remove_object(bucket_name=bucket_name, object_name=name.object_name)
        client.remove_bucket(bucket_name=bucket_name)

    if not client.bucket_exists(bucket_name=bucket_name):
        print("\tCreating bucket ", bucket_name)
        client.make_bucket(bucket_name=bucket_name)


def get_csv_data(n_rows: int):
    buf = "id,created_at,name,data\n"
    for i in range(n_rows):
        buf += f"{i+1},2023-04-01T07:05:{i % 60:02},row-{i+1},{i}\n"
    return buf


clear_bucket("raw")
clear_bucket("clean")
clear_bucket("load-ready")


def create_save_csv(filename: str, bucket_name: str, n_rows: int):
    print(f"Saving file s3://{bucket_name}/{filename} with {n_rows} rows.")
    data = get_csv_data(n_rows)

    client.put_object(
        bucket_name=bucket_name,
        object_name=filename,
        data=BytesIO(data.encode("utf-8")),
        length=len(data)
    )


create_save_csv("data-1.csv", "raw", 25)
create_save_csv("data-2.csv", "raw", 250)
