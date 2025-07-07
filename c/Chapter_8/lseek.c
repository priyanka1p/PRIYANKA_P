
#include <stdio.h> 
#include <unistd.h> 
#include <sys/types.h> 
#include <fcntl.h> 

void func(char arr[], int n) 
{ 
	// Open the file for READ only. 
	int f_write = open("test.txt", O_RDONLY); 

	// Open the file for WRITE and READ only. 
	int f_read=open("end.txt", O_WRONLY);

	lseek(f_write, n, SEEK_SET); 
	read(f_write, arr, 1); 
	write(f_read, arr, 1); 
	lseek(f_write,3,SEEK_CUR);
  read(f_write, arr, 1); 
	write(f_read, arr, 1); 
	
	close(f_write); 
	close(f_read); 
} 


int main() 
{ 
	char arr[100]; 
	int n; 
	n = 2; 
	func(arr, n); 
	return 0; 
} 
