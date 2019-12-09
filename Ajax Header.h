const char MAIN_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<body>


  <h1>Networks Found:<ul><span id="Networks Found">0</span></ul></h1><br>
  

<script>

setInterval(function() {
  // Call a function repetatively with 2 Second interval
  getData();
}, 1000); //1000mSeconds update rate

function getData() {
  var xhttp = new XMLHttpRequest(); //gets part ogf the webpage
  xhttp.onreadystatechange = function() { //event listener .. do this function when event
    if (this.readyState == 4 && this.status == 200) { //ready state has 5 states, unsent, open, sent, loading, done... 4 = done.. status 200 all ok 
      document.getElementById("Networks Found").innerHTML = // get the element on the page referred to by the name
      this.responseText; //comes from server.on response string
    }
  };
  xhttp.open("GET", "Networks", true);
  //Serial.println(this.responseText);
  xhttp.send();
}
</script>
</body>
</html>
)=====";
