dexe() {
  docker \
    exec $(docker ps | grep spark-worker | awk '{print $1}') \
      $1 
      
}

dexe '/bin/bash run-tests.sh'