function addFrame(table, color, transition, duration, position){
	// Create Table
	var frame = table.insertRow(position);
	var colorCell = frame.insertCell(0);
	var transitionCell = frame.insertCell(1);
	var durationCell = frame.insertCell(2); 

	// Create Color Picker
	var colorPicker = document.createElement("button")
	var picker = new jscolor(colorPicker);
	picker.fromString(color);
	colorPicker.style.width = "75px";
	colorPicker.style.height = "20px";

	// Create Transition Dropdown
	var transitionDropdown = document.createElement("select");
	var t1 = new Option();
	var t2 = new Option();
	t1.value = 1;
	t2.value = 2;
	t1.text = "None";
	t2.text = "Fade";
	transitionDropdown.options.add(t1);
	transitionDropdown.options.add(t2);

	// Create Duration Field
	var durationInput = document.createElement("input");
	durationInput.type = "number";
	durationInput.min = 0.1;
	durationInput.max = 10;
	durationInput.step = 0.1;

	// Append them to their respective cells
	colorCell.appendChild(colorPicker);
	transitionCell.appendChild(transitionDropdown);
	durationCell.appendChild(durationInput);
}

function randomHexColor(){
	return Math.floor(Math.random()*16777215).toString(16);
}

window.onload = function(){


	var keyframeTable = document.getElementById("keyframes");

	//If no keyframe data can be found, set up default table with one entry
	addFrame(keyframeTable, "FF0000", "Fade", "10 Sec", keyframeTable.rows.length);

	var newFrameRow = keyframeTable.insertRow(keyframeTable.rows.length);
	var buttonCell = newFrameRow.insertCell(0);
	var newFrameButton = document.createElement("button");
	newFrameButton.innerHTML = "New Keyframe";
	buttonCell.appendChild(newFrameButton);

	newFrameButton.addEventListener("click", function(){ addFrame(keyframeTable, randomHexColor(), 'None', '1 Sec', keyframeTable.rows.length - 1); });


}
