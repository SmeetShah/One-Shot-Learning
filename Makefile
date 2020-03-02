all: oneShotLearning

oneShotLearning: oneShotLearning.c
	gcc -g -Wall -Werror -fsanitize=address -std=c99 oneShotLearning.c -o oneShotLearning

clean:	oneShotLearning
	rm -rf oneShotLearning
