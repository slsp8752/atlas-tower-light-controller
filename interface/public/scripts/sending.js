function parse_query_string(query) {
	var vars = query.split("&");
	var query_string = {};
	for (var i = 0; i < vars.length; i++) {
		var pair = vars[i].split("=");
		// If first entry with this name
		if (typeof query_string[pair[0]] === "undefined") {
			query_string[pair[0]] = decodeURIComponent(pair[1]);
		  // If second entry with this name
	    } 
		
		else if (typeof query_string[pair[0]] === "string") {
			var arr = [query_string[pair[0]], decodeURIComponent(pair[1])];
			query_string[pair[0]] = arr;
			// If third or later entry with this name
		}
		else {
			query_string[pair[0]].push(decodeURIComponent(pair[1]));
		}
	}
	return query_string;
}

window.onload = function(){
	var queryString = window.location.search.substring(1);
	var queries = parse_query_string(queryString);
	console.log(queries);

	var ajaxURLs = [];
	var divs = [];

	for(i in Object.keys(queries)){
		var currentQuery = queries["framepair" + i];
		if(currentQuery != ""){
			var frameDiv = document.createElement('div');
			var divName = "frame" + i;
			frameDiv.id = divName;
			frameDiv.innerHTML = "Waiting for data from frame " + i;
			document.getElementsByTagName('body')[0].appendChild(frameDiv);
			divs.push("#" + divName);
			ajaxURLs.push("/keyStatus?frame=" + queries["framepair" + i]);
		}
	}	

	var responses = [];
	var async_request = [];
	for(i in ajaxURLs){
		(function(i){ // Create a new scope to preserve to value of 'i'. This allows each frame's div to be updated when it's repsective data comes in
			async_request.push($.ajax({ url: ajaxURLs[i]
				 , async: true
				 , type: 'GET'
				 , dataType: 'html'
				})
			.done(function(data) {
		      // Parse response JSON, display results 
			  //if return == 0, success; if return == -1, malformed data; if return == null, timeout
				
			  $(divs[i]).html("Frame " + i + " status: " + data);
			  
			  responses.push(data);
			  console.log("success!");
			  console.log(data);
			})
			.fail(function() {
			  console.log("Something went wrong!");
			}));
		})(i);

	}

	$.when.apply(null, async_request).done(function(){
		console.log("Alldone!");
		console.log(responses);
		// Display "Complete!" message
	});

}
