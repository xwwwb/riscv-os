function killQ(){
	kill $(lsof -t -i tcp:1234)
}

function makeM(){
	ln -s ../build.mk Makefile
}
