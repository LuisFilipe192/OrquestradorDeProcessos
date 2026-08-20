processflow: src/processflow.c src/interpretador.c src/task.c src/job.c
	gcc src/processflow.c src/interpretador.c src/task.c src/job.c -o processflow

make run:
	./processflow

clean:
	rm -f processflow