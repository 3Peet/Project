var app = require('express')();
var http = require('http').Server(app);
var io = require('socket.io')(http);


var express = require('express');
const bodyParser = require('body-parser');


//For write json file
const editJsonFile = require("edit-json-file");
const { getUnpackedSettings } = require('http2');
const e = require('express');
let sw_light_file = editJsonFile(`${__dirname}/src/json/sw-light.json`);
    //sw_light_file.unset(`${__dirname}/src/json/sw-light.json`);



app.set ( "view engine", "ejs" );
app.use(express.static(__dirname + '/public'));

app.use(bodyParser.urlencoded({ extended: true }));
app.use(bodyParser.json());
app.use(bodyParser.raw());


app.get('/',(req,res)=>{
    //res.sendFile(__dirname+'/index.html');
    res.render("index");

})

app.get('/chart',(req,res)=>{
    res.render("chart");
})

app.get('/control',(req,res)=>{
    res.render("control");
})

app.get('/market',(req,res)=>{
    res.render("market");
})

app.post("/start_date_data",(req,res)=>{
    if(req.body.start_date==""){
        console.log("No Data")
        res.render("index");
    }
    else {
        var data="";
        data += "Start Date :"+req.body.start_date;
        console.log(data);
        res.render("index");
    }
    
})



io.on('connect',function(socket){
    socket.on('sw-state',function(data){
        sw_light_file.set("value", data);
        sw_light_file.save();
    });
});


app.get('/sw-light',(req,res)=>{

    res.json(sw_light_file);
})



app.post('/',(req,res)=>{

    if(req.body.id=="postman"){
        io.sockets.emit('tempdata',{value: req.body.temp + '  °C'});
        io.sockets.emit('humdata',{value: req.body.hum + ' %'});
        io.sockets.emit('lightdata',{value: req.body.light + ' lux'});
        io.sockets.emit('ecdata',{value: req.body.ec + ' uS/cm'});
        io.sockets.emit('phdata',{value: req.body.ph});
    }
    
    if(req.body.id=="dht"){
        io.sockets.emit('tempdata',{value: req.body.temp + '  °C'});
        io.sockets.emit('humdata',{value: req.body.hum + ' %'});
    }

    if(req.body.id=="ldr"){
        io.sockets.emit('lightdata',{value: req.body.light + ' lux'});
        
    }

    if(req.body.id=="dfrobot"){
        io.sockets.emit('ecdata',{value: req.body.ec + ' μS/cm'});
        io.sockets.emit('phdata',{value: req.body.ph});
    }

    



    console.log('Got body:', req.body);
    res.sendStatus(200);
    });


    

http.listen(3000,()=>{
    console.log('Start server on port :3000');
})