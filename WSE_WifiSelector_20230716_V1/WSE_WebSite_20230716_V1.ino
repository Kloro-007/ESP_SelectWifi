const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<script>const b_DebugMode = false;</script>

<head>
    <title>ESP Web Server</title>
    <meta name="viewport" content="width=device-width, initial-scale=1" charset="utf-8">
    <style>
        html {
            font-family: Arial;
            display: inline-block;
            text-align: center;
        }

        h2 {
            font-size: 3.0rem;
        }

        p {
            font-size: 3.0rem;
        }

        body {
            max-width: 800px;
            margin: 0px auto;
            padding-bottom: 25px;
        }
    </style>
</head>

<body>
    <form action="/update" method="get">
        <div>
            <label for="IN_Sel">Select wifi:<br></label>
            <select onchange="selChange();" name="IN_Sel" style="min-width:300px;font-family:courier" id="IN_Sel" multiple>
                <option value="none">Looking for available wifi...</option>
            </select>
            <button type="button" onclick="fetchAvailableWifi()">Refresh</button>
            <br>
            <input type="submit" value="Send">
        </div>
    </form>
</body>

</html>

<script type="text/javascript">
    function selChange(){
        // Get the list on the HTML page
        let str_list = document.getElementById("IN_Sel");
        str_list.value = str_list.selectedOptions[str_list.selectedOptions.length-1].value;        
    }

    function fetchAvailableWifi() {
        console.log("Fetching available wifi...");
        if (b_DebugMode) {
            const str_Response =
                '{' +
                '"List": [' +
                '{"ssid":"AVeryVeryLongSsidName", "rssi":-10},' +
                '{"ssid":"Test2", "rssi":0}' +
                ']' +
                '}';
            let res = JSON.parse(str_Response);
            updateListElement(res['List']);
        } else {
            fetch(
                "/getWifiList"
            ).then(
                response => response.json()
            ).then(function (res) {
                updateListElement(res['List']);
            });
        }
        console.log("Done.");
    }

    function updateListElement(listdata) {
        // Get the list on the HTML page
        let str_list = document.getElementById("IN_Sel");

        // Remove current entries
        while (str_list.options.length > 0) {
            str_list.remove(str_list.options.length - 1);
        }

        // Sort the list descending based on signal strength by providing a compare function
        listdata.sort((a, b) => {return b.rssi - a.rssi});

        // Get the SSID with the longest length
        let n_SsidLgth = listdata.map( a => a.ssid.length );
        let n_StrMaxLgth = Math.max.apply(null, n_SsidLgth);

        // Add the new entries from the sorted list
        for (let i = 0; i < listdata.length; i++) {
            var opt = document.createElement('option');

            opt.text = listdata[i].ssid.padEnd(n_StrMaxLgth+1, '.') + listdata[i].rssi.toString().padStart(4, '.');
            opt.value = listdata[i].ssid;

            str_list.add(opt, null);
        }

    }
</script>
)rawliteral";

const char redirect_html[] PROGMEM = R"rawliteral(
<html>
<head>
  <title>ESP Web Server</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    html {font-family: Arial; display: inline-block; text-align: center;}
    h2 {font-size: 3.0rem;}
    p {font-size: 3.0rem;}
    body {max-width: 800px; margin:0px auto; padding-bottom: 25px;}
  </style>
</head>
<body>
    <p>Input successful!</p>
</body>
)rawliteral";
