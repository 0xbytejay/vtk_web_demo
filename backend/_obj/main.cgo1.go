// Code generated by cmd/cgo; DO NOT EDIT.

//line /home/jaywang/Desktop/dev/golang/vtk/cpp/main.go:1:1
package main

/*
#cgo LDFLAGS:-L./foo -Wl,-rpath=./foo -lfoo -lstdc++
#cgo CFLAGS: -I./foo
// #include "bridge.h"

// void goFuncForCallback(image []C.int);
*/
import _ "unsafe"
import "fmt"

func main() {
	// cmd := C.int(1)
	// C.goFuncForCallback()
	// C.bar(C.callbackFuncProto())
}

//export goFuncForCallback
func goFuncForCallback(image [] /*line :20:32*/_Ctype_char /*line :20:38*/) {
	fmt.Println("goFunc", int(*i))
}
