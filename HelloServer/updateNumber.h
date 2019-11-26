#include <WiFi.h>

String updateNumber= R"###(
<!DOCTYPE html>
<html>
<body>

<button onclick="timedText()">Display timed text</button>
<input type="text" id="txt">

<script>

function timedText() {
  var x = document.getElementById("txt");
  
  
  setTimeout(function(){ x.value= "" }, 2000);
  
  setTimeout(function(){ x.value="4 seconds" }, 4000);
  
  setTimeout(function(){ x.value="6 seconds" }, 6000);

</script>

</body>
</html>
)###"; 
