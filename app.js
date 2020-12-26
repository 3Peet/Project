// SECTION
// STUB
// TODO
// LINK
// REVIEW
// ANCHOR
// NOTE
// FIXME


var app = require('express')();
var http = require('http').Server(app);
var io = require('socket.io')(http);
const EventEmitter = require('events')

var express = require('express');
const bodyParser = require('body-parser');
const events = new EventEmitter()

// SECTION For write json file
const editJsonFile = require("edit-json-file");
const { getUnpackedSettings } = require('http2');
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
    
    if(req.body.id=="dht"){
        io.sockets.emit('tempdata',{value: req.body.temp + '  °C'});
        io.sockets.emit('temp_graph',{value: req.body.temp});
        io.sockets.emit('humdata',{value: req.body.hum + ' %'});
    }

    if(req.body.id=="ldr"){
        io.sockets.emit('lightdata',{value: req.body.light + ' lux'});
        io.sockets.emit('phdata',{value: req.body.ph});
    }


    io.on('connect', socket => {
        events.on('temperature', value => {
          socket.emit('temperature', value)
        })
      })
      
      // This function will be changed for the J5 sensor event
      // sensor.on('change', function () {
      //   events.emit('temperature', this.celsius)
      // })
      //
      setInterval(() => {
        const temperature = Math.round(Math.random() * 10)
        events.emit('temperature', temperature)
      }, 5000)

    console.log('Got body:', req.body);
    res.sendStatus(200);
    });


    

http.listen(3000,()=>{
    console.log('Start server on port :3000');
})