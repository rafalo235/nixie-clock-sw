
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

function loadNtp()
{
	location.href = "ntp.html";
}

function loadSynchronize()
{
	location.href = "synchronize.html";
}

function loadTime()
{
	location.href = "time.html";
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
	alert("Connected to AP successfully - IP will appear on display");
}

function onConnectError()
{
	alert("Connection ERROR - please check credentials");
}

function connect()
{
	var passwd = document.getElementById("apn_password").value;
	var e = document.getElementById("apslist");
	var apn = e.options[e.selectedIndex].value;

	sendPost("connect.html", "apn=" + apn + "&passwd=" + passwd,
		onConnectOk, onConnectError);
}

function setupSNTP()
{
	var ntp1 = document.getElementById("ntp1").value;
	var ntp2 = document.getElementById("ntp2").value;
	var ntp3 = document.getElementById("ntp3").value;
	var tz = document.getElementById("tz").value;
	sendPost("ntp.html", "ntp1=" + ntp1 + "&ntp2=" 
		+ ntp2 + "&ntp3=" + ntp3 + "&tz=" + tz,
		loadIndex, loadIndex);
}

function setupTime()
{
	var ye = document.getElementById("ye").value;
	var mo = document.getElementById("mo").value;
	var da = document.getElementById("da").value;
	var ho = document.getElementById("ho").value;
	var mi = document.getElementById("mi").value;
	var se = document.getElementById("se").value;
	sendPost("time.html", "ye=" + ye + "&mo=" 
		+ mo + "&da=" + da + "&ho=" + ho
		+ "&mi=" + mi + "&se=" + se,
		loadIndex, loadIndex);

}

function sendPost(url, params, onOk, onError)
{
	var http = new XMLHttpRequest();
	http.open('POST', url + "?" + params, false);
	//http.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
	http.send(null);
	if(http.status == 200)
	{
  		onOk();
	}
	else
	{
		onError();
	}
	
	/*http.onreadystatechange = function() 
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
	http.send(params);*/
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
