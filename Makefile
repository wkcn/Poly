all:
	g++ *.cpp -o Poly
encode:
	enca -L zh_CN -x UTF-8 ./Data/*.txt
