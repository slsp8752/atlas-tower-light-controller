function addFrame(table, color, position){
	// Create Table
	var frame = table.insertRow(position);
	var colorCell = frame.insertCell(0);
	var transitionCell = frame.insertCell(1);
	var durationCell = frame.insertCell(2); 

	// Create Color Picker
		
	var colorPicker = document.createElement("input");
	colorPicker.setAttribute("type", "hidden");
	colorPicker.setAttribute("name", "frameColor");
	colorPicker.setAttribute("value", color);

	var colorPickerButton = document.createElement("button");
	colorPickerButton.classList.add("inputs");
	var picker = new jscolor(colorPickerButton);
	picker.fromString(color);
	colorPickerButton.style.width = "75px";
	colorPickerButton.style.height = "20px";


	// Create Transition Dropdown
	var transitionDropdown = document.createElement("select");
	transitionDropdown.classList.add("inputs");
	var t1 = new Option();
	var t2 = new Option();
	t1.value = 1;
	t2.value = 2;
	t1.text = "None";
	t2.text = "Fade";
	transitionDropdown.options.add(t1);
	transitionDropdown.options.add(t2);
	transitionDropdown.setAttribute("name", "mode");

	// Create Duration Field
	var durationInput = document.createElement("input");
	durationInput.classList.add("inputs");
	durationInput.type = "number";
	durationInput.min = 0.1;
	durationInput.max = 10;
	durationInput.step = 0.1;
	durationInput.setAttribute("name", "duration");
	durationInput.setAttribute("value", 0);

	// Append them to their respective cells
	colorCell.appendChild(colorPickerButton);
	transitionCell.appendChild(transitionDropdown);
	durationCell.appendChild(durationInput);

	var inputs, index;

	//Getting Mode Number
	modesel = table.rows[1].cells[1].getElementsByTagName("select")[0];
	console.log(modesel.options[modesel.selectedIndex].value);

	//Getting rgb values
	console.log(hexToRgb(table.rows[1].cells[0].innerText).r);

	//Getting duration value
	console.log(table.rows[1].cells[2].getElementsByTagName("input")[0].value);



	inputs = document.getElementsByClassName('inputs');
	for (index = 0; index < inputs.length; ++index) {
		if(inputs[index].nodeName == "BUTTON") console.log(inputs[index].innerHTML);
		else if(inputs[index].nodeName == "SELECT") console.log(inputs[index].options[inputs[index].selectedIndex].text);
		else if(inputs[index].nodeName == "INPUT") console.log(inputs[index].value);
}

	//var queryString = $('#keyframesForm').serialize();
	//console.log(queryString);
	
}

function randomHexColor(){
	return Math.floor(Math.random()*16777215).toString(16);
}

function hexToRgb(hex) {
    var result = /^([a-f\d]{2})([a-f\d]{2})([a-f\d]{2})$/i.exec(hex);
    return result ? {
        r: parseInt(result[1], 16),
        g: parseInt(result[2], 16),
        b: parseInt(result[3], 16)
    } : null;
}

//alert( hexToRgb("#0033ff").g ); // "51";

window.onload = function(){


	var keyframeTable = document.getElementById("keyframes");

	//If no keyframe data can be found, set up default table with one entry
	addFrame(keyframeTable, "FF0000", keyframeTable.rows.length);

	var newFrameRow = keyframeTable.insertRow(keyframeTable.rows.length);
	var buttonCell = newFrameRow.insertCell(0);
	var newFrameButton = document.createElement("button");
	newFrameButton.innerHTML = "New Keyframe";
	buttonCell.appendChild(newFrameButton);

	newFrameButton.addEventListener("click", function(){ addFrame(keyframeTable, randomHexColor(), keyframeTable.rows.length - 1); });

}
