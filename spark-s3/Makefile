.PHONY: clean
default: run

DBT_SPARK_IMAGE=spark-image

built:
	docker build -f ./spark/Dockerfile_base -t "${DBT_SPARK_IMAGE}-base" ./spark
	docker build --no-cache -t "${DBT_SPARK_IMAGE}" ./spark
	touch built

run: built
	docker-compose up --remove-orphans

all: clean run
	echo "Rebuilt all"

clean:
	rm built