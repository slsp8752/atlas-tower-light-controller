window.onload = function(){

	var keyframeTable = document.getElementById("keyframes");

	//IF no keyframe data can be pulled:

	var defaultColor = 'FF0000';
	var defaultTrans = 'Fade';
	var defaultDurat = '10 Sec';

	var defaultFrame = keyframeTable.insertRow(1);
	var defaultColorCell = defaultFrame.insertCell(0);
	var defaultTransCell = defaultFrame.insertCell(1);
	var defaultDuratCell = defaultFrame.insertCell(2);


	var defaultPicker = document.createElement('button')
        var picker = new jscolor(defaultPicker);
		picker.fromString(defaultColor);
		defaultPicker.innerHTML = "";
		defaultPicker.style.width = "50px";
		defaultPicker.style.height = "20px";
    

	console.log(defaultPicker.innerHTML);

	defaultColorCell.appendChild(defaultPicker);
	defaultTransCell.innerHTML = defaultTrans;
	defaultDuratCell.innerHTML = defaultDurat;

}
