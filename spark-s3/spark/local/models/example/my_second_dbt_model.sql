
-- Use the `ref` function to select from other models


{{ 
    config(
        materialized='table',
        file_format='csv',
        options={'header': 'true'},
    ) 
}}

select *
from {{ ref('raw') }}
where id = 1
