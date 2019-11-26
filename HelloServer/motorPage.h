String motorPage = R"###(
<!DOCTYPE html>
<html>
<head>
<style>
.flexDiv {
  background-color: #f1f1f1;
  width: 300px;
  margin: 10px;
  text-align: center;
  line-height: 75px;
  font-size: 30px;
  display: flex;
  flex-direction: row;
}
.flexRow{
  background-color: #d1d1d1;
  width: 100px;
  margin: 10px;
  text-align: center;
  line-height: 75px;
  font-size: 30px;

}
</style>
</head>
<body>
<h1>The flex-direction Property</h1>

<p>The "flex-direction: column;" stacks the flex items vertically (from top to bottom):</p>

<div class="flex-container" style="display: flex; flex-direction: column; background-color: #f1f1f1;">
  <div class="flexDiv">
    <div class="flexRow" style="background-color: #f1f1f1;"></div>
    <div class="flexRow">2</div>
    <div class="flexRow" style="background-color: #f1f1f1;"></div>
  </div>
  <div class="flexDiv">
    <div class="flexRow">4</div>
    <div class="flexRow" style="background-color: #f1f1f1;"></div>
    <div class="flexRow">6</div>
  </div>
  <div class="flexDiv">  
    <div class="flexRow" style="background-color: #f1f1f1;"></div>
    <div class="flexRow">8</div>
    <div class="flexRow" style="background-color: #f1f1f1;"></div>
  </div>
</div>

</body>
</html>
)###";
