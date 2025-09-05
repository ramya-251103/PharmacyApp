CREATE DATABASE `cpp_pharmacymanagementsystem`;

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";

USE `cpp_pharmacymanagementsystem`;

CREATE TABLE `medicine_tb`(
	`id` int(11) NOT NULL,
    `name` varchar(50) NOT NULL DEFAULT '0',
    `company` varchar(50) NOT NULL DEFAULT '0',
    `arrival_date` date NOT NULL,
    `expire_date` date NOT NULL,
    `price` float NOT NULL DEFAULT '0',
    `quantity` int(11) NOT NULL DEFAULT '0'
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

INSERT INTO `medicine_tb` (`id`, `name`, `company`, `arrival_date`, `expire_date`, `price`, `quantity`) VALUES
(1, 'Cardipro - 50', 'Square LTD.', '2016-12-11', '2021-12-30', 60, 16);

ALTER TABLE `medicine_tb`
	ADD PRIMARY KEY (`id`);
    
ALTER TABLE `medicines_tb`
	MODIFY `id` INT(11) NOT NULL AUTO_INCREMENT PRIMARY KEY;
