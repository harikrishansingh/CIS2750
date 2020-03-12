'use strict';

// C library API
const ffi = require('ffi-napi');

// Express App (Routes)
const express = require("express");
const app     = express();
const path    = require("path");
const fileUpload = require('express-fileupload');

app.use(fileUpload());
app.use(express.static(path.join(__dirname+'/uploads')));

// Minimization
const fs = require('fs');
const JavaScriptObfuscator = require('javascript-obfuscator');

// Important, pass in port as in `npm run dev 1234`, do not change
const portNum = process.argv[2];

// Send HTML at root, do not change
app.get('/',function(req,res){
  res.sendFile(path.join(__dirname+'/public/index.html'));
});

// Send Style, do not change
app.get('/style.css',function(req,res){
  //Feel free to change the contents of style.css to prettify your Web app
  res.sendFile(path.join(__dirname+'/public/style.css'));
});

//FIXME: Revert
/*// Send obfuscated JS, do not change
app.get('/index.js',function(req,res){
  fs.readFile(path.join(__dirname+'/public/index.js'), 'utf8', function(err, contents) {
    const minimizedContents = JavaScriptObfuscator.obfuscate(contents, {compact: true, controlFlowFlattening: true});
    res.contentType('application/javascript');
    res.send(minimizedContents._obfuscatedCode);
  });
});*/

// Send obfuscated JS, do not change
app.get('/index.js',function(req,res){
  fs.readFile(path.join(__dirname+'/public/index.js'), 'utf8', function(err, contents) {
    res.contentType('application/javascript');
    res.send(contents);
  });
});

//Respond to POST requests that upload files to uploads/ directory
app.post('/upload', function(req, res) {
  if(!req.files) {
    return res.status(400).send('No files were uploaded.');
  }

  let uploadFile = req.files.uploadFile;

  // Use the mv() method to place the file somewhere on your server
  uploadFile.mv('uploads/' + uploadFile.name, function(err) {
    if(err){
      //If the file saving errored
      return res.status(500).send(err);
    } else {
      //Check if the file is valid
      if (validFile('uploads/' + uploadFile.name)) {
        res.redirect('/');
      } else {
        fs.unlinkSync('uploads/' + uploadFile.name);
        return res.status(400).send("Invalid file.");
      }
    }
  });
});

//Respond to GET requests for files in the uploads/ directory
app.get('/uploads/:name', function(req , res){
  fs.stat('uploads/' + req.params.name, function(err, stat) {
    if(err == null) {
      res.sendFile(path.join(__dirname+'/uploads/' + req.params.name));
    } else {
      console.log('Error in file downloading route: '+err);
      res.send('');
    }
  });
});

//******************** Your code goes here ******************** 
app.listen(portNum);
console.log('Running app at localhost: ' + portNum);

//Get images
app.get('/files', function (req, res) {
  const fs = require('fs');
  const files = fs.readdirSync('uploads');
  //TODO: loop through each file name and creatValidSVG, json stringify the array of json, send it back

  let images = [];
  const library = ffi.Library("./libsvgparse", {'fileToJSON': ['string', ['string', 'string']]});

  //Populate an array wiht information about every SVG image in the uploads directory
  files.forEach(file => {
    var fileData = [];
    var result = JSON.parse(library.fileToJSON('uploads/' + file, "parser/bin/files/svg.xsd"));
    fileData[0] = file;
    fileData[1] = Math.round(fs.statSync("uploads/" + file).size / 1024);
    fileData[2] = result.numRect;
    fileData[3] = result.numCirc;
    fileData[4] = result.numPaths;
    fileData[5] = result.numGroups;
    images.push(fileData);
  });

  res.send({
    data: JSON.stringify(images)
  });
});

app.get('/newFile', function(req, res) {
  const library = ffi.Library("./libsvgparse", {'createEmptySVG': ['bool', ['string']]});
  var result = library.createEmptySVG("uploads/" + req.query.filename);

  if (result) {
    return res.status(200).send({
      filename : req.query.filename
    });
  } else {
    return res.status(400).send({
      filename : req.query.filename
    });
  }
});

function validFile (filePath) {
  const library = ffi.Library("./libsvgparse", {'validateFile': ['bool', ['string', 'string']]});
  return library.validateFile(filePath, "parser/bin/files/svg.xsd");
}