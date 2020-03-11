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
            alert(new Error("Could not load files."));
            populateTable(null);
        }
    });

    //Hides and shows the content table div
    $('.tab.file-log').on("click", function (e) {
        if ($('.file-log-wrapper').css("display") === "none") {
            $('.file-log-wrapper').css("display", "block");
        } else {
            $('.file-log-wrapper').css("display", "none");
        }
        e.preventDefault(); //No redirects if possible
    });

    //Hides and shows the details table div
    $('.tab.details').on("click",function (e) {
        if ($('.detail-wrapper').css("display") === "none") {
            $('.detail-wrapper').css("display", "block");
        } else {
            $('.detail-wrapper').css("display", "none");
        }
        e.preventDefault(); //No redirects if possible
    });

    /*//Show file upload window
    $('#submit-file').on("click",function(){
        var submit = $('#imgupload');
        //Make sure something was selected in file input
        if (!submit.val()) {
            alert("Error: No file specified.");
        } else {
            $.ajax({
                type: 'post',
                url: '/upload',
                dataType: 'json',
                data: JSON.stringify({
                    name: submit.prop('files')[0].name,
                    file: submit.prop('files')[0]
                }),

                success: function (data) {
                    //TODO: Tell user upload success and validate image; reload page
                    alert("success");
                },
                fail: function (error) {
                    //TODO: Display error popup for loading images
                    alert("fail");
                }
            });
        }
    });*/

});

//Populate table
function populateTable (response) {
    console.log(response);

    const table = $('#file-log-table');

    //Show error if files could not be loaded
    if (response === null) {
        table.append("<tr><th colspan=\"7\">Error loading files</th></tr>");
    }

    var images = JSON.parse(response.data); //Now have an array of JSON strings

    console.log(images);

    if (images.length === 0) {
        table.append("<th colspan=\"7\">No files</th>");
    } else {
        //Add the table header
        table.append(
            '<th>Image<br/>(click to view details)</th>\n' +
            '<th>File name<br/>(click to download)</th>\n' +
            '<th>File size</th>\n' +
            '<th>Number of rectangles</th>\n' +
            '<th>Number of circles</th>\n' +
            '<th>Number of paths</th>\n' +
            '<th>Number of groups</th>');

        for (var i = 0; i < images.length; i++) {
            table.append(
                '<tr>' +
                    '<td><img src="' + images[i][0] + '" class="img-thumb"/></td>' +
                    '<td>' + images[i][0] + '</td>' +
                    '<td>' + images[i][1] + '</td>' +
                    '<td>' + images[i][2] + '</td>' +
                    '<td>' + images[i][3] + '</td>' +
                    '<td>' + images[i][4] + '</td>' +
                    '<td>' + images[i][5] + '</td>' +
                '</tr>'
            )
        }
        /*TODO: Populate file log
         *       - Make the thumbnails clickable; call updateDetails*/
    }
}

function updateDetails (image) {
    //TODO: Update details panel with the image's file name as input
}
