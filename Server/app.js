var app = require("express")();
var http = require("http").Server(app);
var io = require("socket.io")(http);

var express = require("express");
const bodyParser = require("body-parser");

//For write json file
const editJsonFile = require("edit-json-file");
const { getUnpackedSettings } = require("http2");
const e = require("express");
let sw_light_file = editJsonFile(`${__dirname}/src/json/sw-light.json`);
let schedule_farm = editJsonFile(`${__dirname}/src/json/schedule.json`);
//sw_light_file.unset(`${__dirname}/src/json/sw-light.json`);

function timeConverter(UNIX_timestamp) {
  var a = new Date(UNIX_timestamp * 1000);
  var months = [
    "Jan",
    "Feb",
    "Mar",
    "Apr",
    "May",
    "Jun",
    "Jul",
    "Aug",
    "Sep",
    "Oct",
    "Nov",
    "Dec",
  ];
  var year = a.getFullYear();
  var month = months[a.getMonth()];
  var date = a.getDate();
  //   var hour = a.getHours();
  //   var min = a.getMinutes();
  //   var sec = a.getSeconds();
  var time =
    // date + " " + month + " " + year + " " + hour + ":" + min + ":" + sec;
    date + " " + month + " " + year;
  return time;
}

function cal_unix_harvest_time(start_date) {
  var data = start_date.split("-");

  var year = parseInt(data[0]);
  var month = parseInt(data[1]);
  var day = parseInt(data[2]);

  unix_year = (year - 1970) * 31556926;
  unix_month = (month - 1) * 2629743.8;
  unix_day = day * 86400;

  unix_45day = 45 * 86400; // 45days of harvest time

  unix_start = unix_year + unix_month + unix_day; // Start date
  unix_done = unix_year + unix_month + unix_day + unix_45day; // End date

  return unix_done;
}

function cal_days_remaining(unix_done) {
  var unix_now = Math.round(new Date().getTime() / 1000);
  var days_remaining = Math.round((unix_done - unix_now) / 86400);
  if(days_remaining<=0) return 0;
  else return days_remaining;
  
}

app.set("view engine", "ejs");
app.use(express.static(__dirname + "/public"));

app.use(bodyParser.urlencoded({ extended: true }));
app.use(bodyParser.json());
app.use(bodyParser.raw());

app.get("/", (req, res) => {
  //res.sendFile(__dirname+'/index.html');
  res.render("index");
});

app.get("/chart", (req, res) => {
  res.render("chart");
});

app.get("/control", (req, res) => {
  res.render("control");
});

app.get("/market", (req, res) => {
  res.render("market");
});

app.get("/about", (req, res) => {
  res.render("about");
});

app.post("/start_date_data", (req, res) => {
  if (req.body.start_date == "") {
    console.log("Schedule : No Data");
  } else {
    var data = "";
    data += req.body.start_date;
    schedule_farm.set("start_date", data);
    schedule_farm.save();

    schedule_farm.set("day_remaining", cal_days_remaining(cal_unix_harvest_time(data)));
    schedule_farm.save();

    console.log("Schedule Status : " + schedule_farm.get("status"));
    console.log("Start date : " + data);
    console.log("End date : " + timeConverter(cal_unix_harvest_time(data)));
    console.log(
      "Days to Harvest : " + cal_days_remaining(cal_unix_harvest_time(data))
    );

    io.sockets.emit(
      "schedule",
      { status: schedule_farm.get("status") },
      { start_date: schedule_farm.get("start_date") },
      { days_remaining: schedule_farm.get("day_remaining")}

    );
  }
});

io.on("connect", function (socket) {
  socket.on("sw-state", function (data) {
    sw_light_file.set("value", data);
    sw_light_file.save();
  });

  socket.on("schedule_status", function (data) {
    schedule_farm.set("status", data);
    schedule_farm.save();
  });

  io.sockets.emit(
    "schedule",
    { status: schedule_farm.get("status") },
    { start_date: schedule_farm.get("start_date") },
    { days_remaining: schedule_farm.get("day_remaining")}
  );
  console.log("Status : " + schedule_farm.get("status"));
});

app.get("/sw-light", (req, res) => {
  res.json(sw_light_file);
});

app.post("/", (req, res) => {
  if (req.body.id == "postman") {
    io.sockets.emit("tempdata", { value: req.body.temp + "  °C" });
    io.sockets.emit("humdata", { value: req.body.hum + " %" });
    io.sockets.emit("lightdata", { value: req.body.light + " lux" });
    io.sockets.emit("ecdata", { value: req.body.ec + " uS/cm" });
    io.sockets.emit("phdata", { value: req.body.ph });
  }

  if (req.body.id == "dht") {
    io.sockets.emit("tempdata", { value: req.body.temp + "  °C" });
    io.sockets.emit("humdata", { value: req.body.hum + " %" });
    io.sockets.emit("ecdata", { value: req.body.ec + " mS/cm" });
    io.sockets.emit("phdata", { value: req.body.ph });
    io.sockets.emit("water_lvl",{value: req.body.water + " %"});

    io.sockets.emit(
      "dht_data",
      { temp_data: req.body.temp },
      { hum_data: req.body.hum },
      { ec_data: req.body.ec},
      { ph_data: req.body.ph}
      
    );
  }

  if (req.body.id == "ldr") {
    io.sockets.emit("lightdata", { value: req.body.light + " lux" });
  }

  if (req.body.id == "dfrobot") {
    io.sockets.emit("ecdata", { value: req.body.ec + " μS/cm" });
    io.sockets.emit("phdata", { value: req.body.ph });
  }

  console.log("Got body:", req.body);
  res.sendStatus(200);
});

http.listen(3000, () => {
  console.log(`HydroPlant app listening at http://localhost:3000`);
});
