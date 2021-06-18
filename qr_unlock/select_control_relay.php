<?php
include_once "connection.php";

class emp
{
}

$user = $_POST['user'];
$pass = $_POST['pass'];

$result = mysqli_query($con, "SELECT * FROM `user` where `user` = '$user' and `pass` = '$pass'") or trigger_error(die(json_encode($response)), E_USER_ERROR);
if ($result->num_rows > 0) {
    while ($row = $result->fetch_assoc()) {
        $name =  $row["name"];
        $room =  $row["room"];
        $query = mysqli_query($con, "INSERT INTO `history`(`name`, `room`) VALUES ('$name','$room')");
        if ($query) {
            $response = new emp();
            $response->success = 1;
            $response->message = "Successfully Uploaded";
            die(json_encode($response));
        } else {
            $response = new emp();
            $response->success = 0;
            $response->message = "Error Upload";
            die(json_encode($response));
        }
    }
} else {
    $response = new emp();
    $response->success = 0;
    $response->message = "Error Upload";
    die(json_encode($response));
}
mysqli_close($con);
