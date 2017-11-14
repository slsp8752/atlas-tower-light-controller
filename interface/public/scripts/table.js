function addFrame(table, color, position, newFrameButton){

	if(table.rows.length > 11) return;

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
	durationInput.setAttribute("value", 1000);

	// Append them to their respective cells
	colorCell.appendChild(colorPickerButton);
	transitionCell.appendChild(transitionDropdown);
	durationCell.appendChild(durationInput);

	var deleteButton = document.createElement("button");
	deleteButton.innerText = "Delete";
	deleteButton.onclick = function() {deleteFrame(table, deleteCell);};
	deleteCell.appendChild(deleteButton);	

	if(table.rows.length > 11) newFrameButton.disabled = true;

	else if(table.rows.length < 3) table.rows[1].cells[3].getElementsByTagName("button")[0].disabled = true;
	else table.rows[1].cells[3].getElementsByTagName("button")[0].disabled = false;

	
}

function deleteFrame(table, cell){
	var index = cell.parentElement.rowIndex;
	if(table.rows.length > 3) table.deleteRow(cell.parentElement.rowIndex);
	if(table.rows.length < 4) table.rows[1].cells[3].getElementsByTagName("button")[0].disabled = true;
	if(table.rows.length < 12) table.rows[table.rows.length-1].cells[0].getElementsByTagName("button")[0].disabled = false; 

}

function rowToKeyframe(table, rowNumber){
	var rgb = hexToRgb(table.rows[rowNumber].cells[0].innerText);

	var modeSel = table.rows[rowNumber].cells[1].getElementsByTagName("select")[0];
	var mode = modeSel.options[modeSel.selectedIndex].value;

	var dur = ("00000" + table.rows[rowNumber].cells[2].getElementsByTagName("input")[0].value).substr(-5,5);

	return ("k" + (rowNumber-1) + "r" + rgb.r + "g" + rgb.g + "b" + rgb.b + "m" + mode + "t" + dur);

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
	var newButtonCell = newFrameRow.insertCell(0);
	var submitButton = document.createElement("button");
	var newFrameButton = document.createElement("button");
	var buttonDiv = document.createElement("div");

	buttonDiv.className = "divider"; 
	newFrameButton.innerHTML = "New Keyframe";
	submitButton.innerHTML = "Send Keyframes";

	newButtonCell.appendChild(newFrameButton);
	newButtonCell.appendChild(buttonDiv);
	newButtonCell.appendChild(submitButton);

	newFrameButton.addEventListener("click", function(){
		addFrame(keyframeTable, randomHexColor(), keyframeTable.rows.length - 1, newFrameButton);
	});

	//framepair1=k1...k2...&framepair2=k3...k4...&framepair3=k5...k6...&framepair4=k7...k8... ...

	submitButton.addEventListener("click", function(){
		//var keyframesString = "";
		var framepairs = ["", "", "", "", ""]; //Empty strings will be included in the final set of args so the server can check if they're empty, and only make a post request for non-empty args

		for(i = 1; i < keyframeTable.rows.length-1; i++){
			framepairs[Math.ceil(i/2)-1] += rowToKeyframe(keyframeTable, i); //Map every two keyframes to each entry in framepairs
			if(i == keyframeTable.rows.length-2) framepairs[Math.ceil(i/2)-1] += 'F';
		}
		var argString = "/send?framepair0="+ framepairs[0] + "&framepair1="+ framepairs[1] + "&framepair2="+ framepairs[2] + "&framepair3="+ framepairs[3] + "&framepair4="+ framepairs[4]; 
		//window.location.href='/send?keyframes='+keyframesString;
		window.location.href = argString;
		//var query = window.location.search.substring(1);
		//var qs = parse_query_string(query);
	});

}
