
function clearSelect(id)
{
	var select = document.getElementById(id);
	var length = select.options.length;
	for (i = length - 1; i >= 0; i--) {
	  select.options[i] = null;
	}
}

function fillSelect(id, arr)
{
	var select = document.getElementById(id);
	for (i in arr.acess_points) {
		var opt = document.createElement('option');
		opt.value = arr.acess_points[i].value;
		opt.innerHTML = arr.acess_points[i].display;
		select.appendChild(opt);
	}
}

function loadStatus()
{
	location.href = "status.html";
}

function loadConnect()
{
	location.href = "connect.html";
}

function loadDisconnect()
{
	location.href = "disconnect.html";
}

function loadIndex()
{
	location.href = "index.html";
}

function refreshConnect()
{	
	var xhr = new XMLHttpRequest();
    xhr.open('GET', 'accesspoints.json', true);
    xhr.responseType = 'json';
    xhr.onload = function() {
		var status = xhr.status;
		var aps = xhr.response;
		
		if (status === 200) {
			clearSelect("apslist");
			fillSelect("apslist", aps);
		} else {
			displayError('Could not refresh list');
		}
    };
    xhr.send();
}

function sendConnect()
{
	var select = document.getElementById("apslist");
	var selected = select.options[select.selectedIndex].value;
	
	var modal = document.getElementById("passwordpopup");
	modal.style.display = "block";
}

function displayError(textMessage)
{
	var modal = document.getElementById("error-popup");
	var message = document.getElementById("error-message");
	var passwd = document.getElementById("passwordpopup");

	passwd.style.display = "none";	
	modal.style.display = "block";
	message.innerHTML = textMessage;
}

function onConnectOk()
{
	alert("OK");
}

function onConnectError()
{
	alert("ERROR");
}

function connect()
{
	var passwd = document.getElementById("apn_password").value;
	var e = document.getElementById("apslist");
	var apn = e.options[e.selectedIndex].value;

	sendPost("connect.html", "apn=" + apn + "&passwd=" + passwd,
		onConnectOk, onConnectError);
}

function sendPost(url, params, onOk, onError)
{
	var http = new XMLHttpRequest();
	http.open("POST", url, true);
	http.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
	http.onreadystatechange = function() 
	{
    	if(http.readyState == 4 && http.status == 200) 
    	{
        	onOk();
    	}
    	else
    	{
    		onError();
    	}
	};
	http.send(params);
}

window.onclick = function(event) {
	var modal = document.getElementById("passwordpopup");
	var errorModal = document.getElementById("error-popup");
    if (event.target == modal) {
        modal.style.display = "none";
    }
	if (event.target == errorModal) {		
        errorModal.style.display = "none";
	}
}
