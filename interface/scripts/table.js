function addFrame(table, color, position, newFrameButton){

	// Create Row
	var frame = table.insertRow(position);
	var colorCell = frame.insertCell(0);
	var transitionCell = frame.insertCell(1);
	var durationCell = frame.insertCell(2); 
	var deleteCell = frame.insertCell(3);

	// Create Color Picker
		
	var colorPicker = document.createElement("input");
	colorPicker.setAttribute("type", "hidden");
	colorPicker.setAttribute("name", "frameColor");
	colorPicker.setAttribute("value", color);

	var colorPickerButton = document.createElement("button");
	var picker = new jscolor(colorPickerButton);
	picker.fromString(color);
	colorPickerButton.style.width = "75px";
	colorPickerButton.style.height = "20px";


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
	transitionDropdown.setAttribute("name", "mode");

	// Create Duration Field
	var durationInput = document.createElement("input");
	durationInput.type = "number";
	durationInput.min = 1;
	durationInput.max = 10000;
	durationInput.setAttribute("name", "duration");
	durationInput.setAttribute("value", 0);

	// Append them to their respective cells
	colorCell.appendChild(colorPickerButton);
	transitionCell.appendChild(transitionDropdown);
	durationCell.appendChild(durationInput);

	if(table.rows.length > 3){
		var deleteButton = document.createElement("button");
		deleteButton.innerText = "Delete";
		deleteButton.onclick = function() {table.deleteRow(getRowNumber(deleteCell));};
		deleteCell.appendChild(deleteButton);	
	}

	if(table.rows.length == 12) newFrameButton.disabled = true;


	console.log(rowToKeyframe(table, 1));
	
}

function getRowNumber(cell){
	return cell.parentElement.rowIndex; 
}

function rowToKeyframe(table, rowNumber){
	rgb = hexToRgb(table.rows[rowNumber].cells[0].innerText);

	modeSel = table.rows[rowNumber].cells[1].getElementsByTagName("select")[0];
	mode = modeSel.options[modeSel.selectedIndex].value;

	dur = ("00000" + table.rows[rowNumber].cells[2].getElementsByTagName("input")[0].value).substr(-5,5);

	return ("k" + rowNumber + "r" + rgb.r + "g" + rgb.g + "b" + rgb.b + "m" + mode + "t" + dur);

}

function randomHexColor(){
	return Math.floor(Math.random()*16777215).toString(16);
}

function hexToRgb(hex) {
    var result = /^([a-f\d]{2})([a-f\d]{2})([a-f\d]{2})$/i.exec(hex);
    return result ? {
        r: ("000" + parseInt(result[1], 16)).substr(-3,3),
        g: ("000" + parseInt(result[2], 16)).substr(-3,3),
        b: ("000" + parseInt(result[3], 16)).substr(-3,3)
    } : null;
}

window.onload = function(){


	var keyframeTable = document.getElementById("keyframes");

	//If no keyframe data can be found, set up default table with one entry
	addFrame(keyframeTable, "FF0000", keyframeTable.rows.length);

	var newFrameRow = keyframeTable.insertRow(keyframeTable.rows.length);
	var buttonCell = newFrameRow.insertCell(0);
	var newFrameButton = document.createElement("button");
	newFrameButton.innerHTML = "New Keyframe";
	buttonCell.appendChild(newFrameButton);

	newFrameButton.addEventListener("click", function(){ addFrame(keyframeTable, randomHexColor(), keyframeTable.rows.length - 1, newFrameButton); });

}
