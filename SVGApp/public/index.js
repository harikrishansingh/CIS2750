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
            //TODO: Display error popup for loading images
        }
    });

    //Hides and shows the content table div
    $('.tab.file-log').click(function (e) {
        if ($('.file-log-wrapper').css("display") === "none") {
            $('.file-log-wrapper').css("display", "block");
        } else {
            $('.file-log-wrapper').css("display", "none");
        }
        e.preventDefault(); //No redirects if possible
    });

    //Hides and shows the details table div
    $('.tab.details').click(function (e) {
        if ($('.detail-wrapper').css("display") === "none") {
            $('.detail-wrapper').css("display", "block");
        } else {
            $('.detail-wrapper').css("display", "none");
        }
        e.preventDefault(); //No redirects if possible
    });
});

//Populate table
function populateTable (response) {
    let fileNames = response.files; //Array of all file names

    if (fileNames.length === 0) {
        //TODO: Put empty in file log
    } else {
        /*TODO: Populate file log
         *       - Make the thumbnails clickable; call updateDetails*/
    }
}

function updateDetails (image) {
    //TODO: Update details panel with the image's file name as input
}
