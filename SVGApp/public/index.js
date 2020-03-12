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

        const selector = $('#detail-select');
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
            selector.append('<option value="' + images[i][0] + '" class="image-option ' + images[i][0] + '">' + images[i][0] + '</option>');
        }
    }
}

function updateDetails (image) {
    //Update the details select box and details table
    $('#detail-select').val(image);
    $('.detail-wrapper').css("display", "block");
    const table = $('.details-table');

    //Clear the table first
    table.empty();

    table.append(
        '<tr><td colspan="6" rowspan="2"><img src="' + image + '" width="800px" /></td></tr>' +
        '<tr></tr>' +
        '<tr><td colspan="2" class="detail-heading"><b>Title</b></td><td colspan="4" class="detail-heading"><b>Description</b></td></tr>' +
        //TODO: Get actual title and description
        '<tr><td colspan="2">THIS IS A TITLE</td><td colspan="4">THIS IS A DESCRIPTION</td></tr>' +
        '<tr><td class="detail-heading"><b>Component</b></td><td colspan="4" class="detail-heading"><b>Summary</b></td><td class="other-attributes detail-heading"><b>Other attributes</b></td></tr>'
    );

    //TODO: Loop and get the actual data
    table.append('<tr><td>Circle</td><td colspan="4">Center: x=3cm, y=1cm Radius: 1cm</td><td class="other-attributes">0</td></tr>');

}
