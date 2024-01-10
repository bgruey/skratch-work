.PHONY: clean
default: run

DBT_SPARK_IMAGE=spark-image

built:
	docker build -f ./spark/Dockerfile_base -t "${DBT_SPARK_IMAGE}-base" ./spark
	touch built
reload: built
	docker build --no-cache -t "${DBT_SPARK_IMAGE}" ./spark

run: reload
	docker-compose up --remove-orphans

all: clean run
	echo "Rebuilt all"

clean:
	docker image rm -f "${DBT_SPARK_IMAGE}"
	rm built