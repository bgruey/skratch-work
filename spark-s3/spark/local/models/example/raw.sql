
/*
    Welcome to your first dbt model!
    Did you know that you can also configure models directly within SQL files?
    This will override configurations stated in dbt_project.yml

    Try changing "table" to "view" below
*/

{{ 
    config(
        materialized='view',
        file_format='csv',
        location_root="s3://minio-s3:9000/raw"
    ) 
}}



select id, created_at, name, data
from {{ source('source', 'data-1.csv') }}