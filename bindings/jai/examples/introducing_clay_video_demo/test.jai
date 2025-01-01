#import "Basic";


test :: () -> bool #must #expand {
	print("one\n");
	
	`defer print("two\n");

	return true;
}

main :: () {
	print("ichi\n");
	if test() {
		print("ni\n");
		if test() {
			print("san\n");
		}
	}
	print("yon\n");
}