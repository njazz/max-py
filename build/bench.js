function bang(){
	outlet(0, 1+1+2*4+1+1+2*4)
	}
	
function msg_int(i)
{
	outlet(0, i+1+2*4+1+1+2*4)
	}
	
function make_list()
{
	var lst = new Array();
	
	for (var i=0;i<100;i++){
		lst[i] =  i % 3 + 1+2;//Math.random();
		}
	
	outlet(0,lst);
	}