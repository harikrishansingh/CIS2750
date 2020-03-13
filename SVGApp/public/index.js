// Put all onload AJAX calls here, and event listeners
$(document).ready(function () {
    //Gets file names to populate file log panel with
    $.ajax({
        type: 'get',
        dataType: 'json',
        url: '/files',
        data: {},
        success: function (data) {
            populateTable(data);
        },
        fail: function (error) {
            alert(new Error("Could not load files." + error));
            populateTable(null);
        }
    });

    //Make new file
    $('#new-file').on("click", function () {
        const fileURL = prompt("Enter a file name", "");

        if (!(fileURL == null || fileURL === "")) {
            $.ajax({
                type: 'get',
                dataType: 'json',
                url: '/newFile',
                data: {
                    filename: fileURL
                },
                success: function (data) {
                    alert("New file " + data.filename + " created!");
                    location.reload();
                },
                fail: function (error) {
                    alert(new Error("Could not create new file." + error));
                }
            });
        } else {
            alert("New file cancelled.");
        }
    });

    //Hides and shows the content table div
    $('.tab.file-log').on("click", function (e) {
        const flw = $('.file-log-wrapper');
        if (flw.css("display") === "none") {
            flw.css("display", "block");
        } else {
            flw.css("display", "none");
        }
        e.preventDefault(); //No redirects if possible
    });

    //Save the image when the save button is pressed
    $('#saveComponents').on("click", function(e) {
        //TODO: save title, description, all component data, all components
    })
});

//Populate table
function populateTable (response) {
    const table = $('#file-log-table');

    //Show error if files could not be loaded
    if (response === null) {
        table.append("<tr><th colspan=\"7\">Error loading files</th></tr>");
        return;
    }

    const images = JSON.parse(response.data); //Now have an array of JSON strings

    if (images.length === 0) {
        table.append("<th colspan=\"7\">No files</th>");
    } else {
        //Add the table header
        table.append(
            '<th>Image<br/>(click to view details)</th>' +
            '<th>File name<br/>(click to download)</th>' +
            '<th>File size</th>' +
            '<th>Number of rectangles</th>' +
            '<th>Number of circles</th>' +
            '<th>Number of paths</th>' +
            '<th>Number of groups</th>');

        for (var i = 0; i < images.length; i++) {
            //Add SVGs to log table
            table.append(
                '<tr>' +
                '<td class="image"><img src="' + images[i][0] + '" class="img-thumb ' + images[i][0] + '" onclick="updateDetails(\'' + images[i][0] + '\')"/></td>' +
                '<td class="file-name"><a href="' + images[i][0] + '">' + images[i][0] + '</td>' +
                '<td class="file-size">' + images[i][1] + '</td>' +
                '<td class="numRects">' + images[i][2] + '</td>' +
                '<td class="numCircles">' + images[i][3] + '</td>' +
                '<td class="numPaths">' + images[i][4] + '</td>' +
                '<td class="numGroups">' + images[i][5] + '</td>' +
                '</tr>'
            );

            //Add SVGs to details selector
            $('#detail-select').append('<option value="' + images[i][0] + '" class="image-option ' + images[i][0] + '">' + images[i][0] + '</option>');
        }
    }
}

function updateDetails (image) {
//Update the details select box and details table
    $('#detail-select').val(image);
    $('.detail-wrapper').css("display", "block");
    $('#component-save-edit-wrapper').css("display", "block");
    $('#selectShapeNumber').css("display", "none");
    $('#attributeTable').css("display", "none");
    $('#selectComponent').empty();
    $('#selectComponent').append('<option disabled selected value>-- Select an element type--</option>');
    $('#selectRectNumber').empty();
    $('#selectRectNumber').append('<option disabled selected value>-- Select an element number--</option>');
    $('#selectCircleNumber').empty();
    $('#selectCircleNumber').append('<option disabled selected value>-- Select an element number--</option>');
    $('#selectPathNumber').empty();
    $('#selectPathNumber').append('<option disabled selected value>-- Select an element number--</option>');
    const table = $('.details-table');

    //Get our JSON string for a file
    let imageJSON;
    $.ajax({
        type: 'get',
        dataType: 'json',
        url: '/fileData',
        async: false,
        data: {
            filename: image
        },
        success: function (data) {
            imageJSON = data;
        },
        fail: function (error) {
            alert(new Error("Could not load data for file. " + error));
        }
    });

    if (imageJSON === undefined || imageJSON === "") {
        $('#detail-select').val(image);
        $('.detail-wrapper').css("display", "none");
        return;
    }

    //Clear the table first
    table.empty();

    //Add titles and content
    table.append(
        '<tr><td colspan="6" rowspan="2"><img id="imageInFocus" src="' + image + '" width="800px" /></td></tr>' +
        '<tr></tr>' +
        '<tr><td colspan="2" class="detail-heading"><b>Title</b></td><td colspan="4" class="detail-heading"><b>Description</b></td></tr>' +
        '<tr><td colspan="2"><textarea class="imageDescriptor title" id="' + image + '" maxlength="255">' + imageJSON.title + '</textarea><button onclick="saveTitle()">Save Title</button></td>' +
        '<td colspan="4"><textarea class="imageDescriptor description" id="' + image + '" maxlength="255">' + imageJSON.description + '</textarea><button onclick="saveDescription()">Save Description</button></td></tr>' +
        '<tr><td class="detail-heading"><b>Component</b></td><td colspan="4" class="detail-heading"><b>Summary</b></td><td class="other-attributes detail-heading"><b>Other attributes</b></td></tr>'
    );

    let i;
    let select;
    //Loop through rectangles
    imageJSON.rectangles.forEach(function(r, i)  {
        table.append('<tr><td>Rectangle ' + (i - -1) + '</td><td colspan="4">Top left: x=' + r.x + r.units + ', y=' + r.y + r.units + '<br>' +
            'Width: ' + r.w + r.units + ' Height: ' + r.h + r.units + '</td><td class="other-attributes">' + r.numAttr + '</td></tr>')
    });
    //Add to selector for editing
    if (imageJSON.rectangles.length > 0) {
        $('#selectComponent').append('<option value="rects" class="element-option">Rectangles</option>')
        select = $('#selectRectNumber');
        for(i = 0; i < imageJSON.rectangles.length; i++){
            select.append('<option value="' + i + '" class="element-option">Rectangle ' + (i - -1) + '</option>');
        }
    }

    //Loop through circles
    imageJSON.circles.forEach(function(c, i)  {
        table.append('<tr><td>Circle ' + (i - -1) + '</td><td colspan="4">Center: cx=' + c.cx + c.units + ', cy=' + c.cy + c.units + '<br>' +
            'Radius: ' + c.r + c.units + '</td><td class="other-attributes">' + c.numAttr + '</td></tr>')
    });
    //Add to selector for editing
    if (imageJSON.circles.length > 0) {
        $('#selectComponent').append('<option value="circles" class="element-option">Circles</option>')
        select = $('#selectCircleNumber');
        for(i = 0; i < imageJSON.circles.length; i++){
            select.append('<option value="' + i + '" class="element-option">Circle ' + (i - -1) + '</option>');
        }
    }

    //Loop through paths
    imageJSON.paths.forEach(function(p, i)  {
        table.append('<tr><td>Path ' + (i - -1) + '</td><td colspan="4">Data: ' + p.d + '</td><td class="other-attributes">' + p.numAttr + '</td></tr>')
    });
    //Add to selector for editing
    if (imageJSON.paths.length > 0) {
        $('#selectComponent').append('<option value="paths" class="element-option">Paths</option>')
        select = $('#selectPathNumber');
        for(i = 0; i < imageJSON.paths.length; i++){
            select.append('<option value="' + i + '" class="element-option">Path ' + (i - -1) + '</option>');
        }
    }

    //Loop through groups
    imageJSON.groups.forEach(function(g, i)  {
        table.append('<tr><td>Group ' + (i - -1) + '</td><td colspan="4">Number of children: ' + g.children + '</td><td class="other-attributes">' + g.numAttr + '</td></tr>')
    });
}

function saveTitle() {
    const title = $('.title').val();
    const imageName = $('#imageInFocus').attr('src');
    $.ajax({
        type: 'get',
        dataType: 'json',
        url: '/saveTitle',
        data: {
            imageName: imageName,
            title: title
        },
        success: function () {
            alert("Title saved!");
        },
        fail: function (error) {
            alert(new Error("Could not save description. " + error));
        }
    });
}

function saveDescription() {
    const description = $('.description').val();
    const imageName = $('#imageInFocus').attr('src');
    $.ajax({
        type: 'get',
        dataType: 'json',
        url: '/saveDesc',
        data: {
            imageName: imageName,
            description: description
        },
        success: function () {
            alert("Description saved!");
        },
        fail: function (error) {
            alert(new Error("Could not save description. " + error));
        }
    });
}

function showShapes(type) {
    let rectSelect = $('#selectRectNumber');
    let circleSelect = $('#selectCircleNumber');
    let pathSelect = $('#selectPathNumber');

    if (type === "rects"){
        rectSelect.css("display", "block");
        circleSelect.css("display", "none");
        pathSelect.css("display", "none");
    } else if (type === "circles") {
        rectSelect.css("display", "none");
        circleSelect.css("display", "block");
        pathSelect.css("display", "none");
    } else if (type === "paths") {
        rectSelect.css("display", "none");
        circleSelect.css("display", "none");
        pathSelect.css("display", "block");
    }
    $('#attributeTable').css("display", "none");
    $('#saveAttributes').css("display", "none");
}

function showAttrs(shapeNumber, shape) {
    var shapes;
    const imageName = $('#imageInFocus').attr('src');
    $.ajax({
        type: 'get',
        dataType: 'json',
        url: '/fileData',
        async: false,
        data: {
            filename: imageName
        },
        success: function (r) {
            shapes = r;
        },
        fail: function (error) {
            alert(new Error("Could not get shapes. " + error));
            location.reload();
        }
    });

    //Populate the attribute table with all the attributes of the selected element
    var table = $('#attributeTable');
    table.empty();
    table.append('<tr><th class="heading"><h2>Attribute</h2></th><th class="heading"><h2>Value</h2></th></tr>');

    //This is kinda gross and very duplicate codey but each part has something slightly different
    let s;
    if (shape === "circles") {
        s = shapes.circles[shapeNumber];
        table.append(
            '<tr><td>cx</td><td>' + s.cx + '</td></tr>' +
            '<tr><td>cy</td><td>' + s.cy + '</td></tr>' +
            '<tr><td>r</td><td>' + s.r + '</td></tr>' +
            '<tr><td>units</td><td>' + s.units + '</td></tr>');

        if (s.numAttr > 0) table.append('<tr><td colspan="2"><b>Other Attribute(s)</b></td></tr>');
        s.otherAttrs.forEach(function(oa) {
            table.append(
                '<tr><td>' + oa.name + '</td><td>' + oa.value + '</td></tr>'
            )
        })
    } else if (shape === "rects") {
        s = shapes.rectangles[shapeNumber];
        table.append(
            '<tr><td>x</td><td>' + s.x + '</td></tr>' +
            '<tr><td>y</td><td>' + s.y + '</td></tr>' +
            '<tr><td>width</td><td>' + s.w + '</td></tr>' +
            '<tr><td>height</td><td>' + s.h + '</td></tr>'+
            '<tr><td>units</td><td>' + s.units + '</td></tr>');

        if (s.numAttr > 0) table.append('<tr><td colspan="2"><b>Other Attribute(s)</b></td></tr>');
        s.otherAttrs.forEach(function(oa) {
            table.append(
                '<tr><td>' + oa.name + '</td><td>' + oa.value + '</td></tr>'
            )
        })
    } else if (shape === "paths") {
        s = shapes.paths[shapeNumber];
        table.append('<tr><td>d</td><td>' + s.d + '</td></tr>');

        if (s.numAttr > 0) table.append('<tr><td colspan="2"><b>Other Attribute(s)</b></td></tr>');
        s.otherAttrs.forEach(function(oa) {
            table.append(
                '<tr><td>' + oa.name + '</td><td>' + oa.value + '</td></tr>'
            )
        })
    }

    table.css("display", "inline-table");
    $('#saveAttributes').css("display", "block")
}
