get_tweets.out: mylibs.o get_tweets.o parse_output.out
	gcc mylibs.o get_tweets.o -lcurl -lssl -lcrypto -o get_tweets.out -g
mylibs.o: mylibs.c mylibs.h
	gcc -c mylibs.c -g
get_tweets.o: get_tweets.c mylibs.h
	gcc -c get_tweets.c -g

parse_output.out: parse_output.o analyse_sentiment.o
	gcc parse_output.o analyse_sentiment.o -o parse_output.out -g
parse_output.o: parse_output.c analyse_sentiment.h
	gcc -c parse_output.c -g
analyse_sentiment.o: analyse_sentiment.c analyse_sentiment.h
	gcc -c analyse_sentiment.c -g