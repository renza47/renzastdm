-- phpMyAdmin SQL Dump
-- version 5.1.1
-- https://www.phpmyadmin.net/
--
-- Host: 127.0.0.1
-- Generation Time: Sep 24, 2023 at 07:08 AM
-- Server version: 10.4.22-MariaDB
-- PHP Version: 8.1.1

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
START TRANSACTION;
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Database: `rtdm`
--

-- --------------------------------------------------------

--
-- Table structure for table `bans`
--

CREATE TABLE `bans` (
  `ban_id` int(11) NOT NULL,
  `user_id` int(11) NOT NULL,
  `username` varchar(24) COLLATE utf8mb4_unicode_ci NOT NULL,
  `admin_id` int(11) NOT NULL,
  `admin_name` varchar(24) COLLATE utf8mb4_unicode_ci NOT NULL,
  `reason` varchar(128) COLLATE utf8mb4_unicode_ci NOT NULL,
  `ban_date` varchar(60) COLLATE utf8mb4_unicode_ci NOT NULL,
  `ip` varchar(60) COLLATE utf8mb4_unicode_ci NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

-- --------------------------------------------------------

--
-- Table structure for table `players`
--

CREATE TABLE `players` (
  `id` int(11) NOT NULL,
  `username` varchar(24) COLLATE utf8mb4_unicode_ci NOT NULL,
  `password` char(64) COLLATE utf8mb4_unicode_ci NOT NULL,
  `salt` char(16) COLLATE utf8mb4_unicode_ci NOT NULL,
  `kills` mediumint(9) NOT NULL DEFAULT 0,
  `deaths` mediumint(9) NOT NULL DEFAULT 0,
  `x` float NOT NULL DEFAULT 0,
  `y` float NOT NULL DEFAULT 0,
  `z` float NOT NULL DEFAULT 0,
  `angle` float NOT NULL DEFAULT 0,
  `interior` tinyint(4) NOT NULL DEFAULT 0,
  `registered` tinyint(4) NOT NULL DEFAULT 0,
  `skin` int(11) NOT NULL DEFAULT 299,
  `admin_level` int(11) NOT NULL DEFAULT 0,
  `score` int(11) NOT NULL DEFAULT 0,
  `cash` int(11) NOT NULL DEFAULT 250,
  `weapon1` int(11) NOT NULL DEFAULT 32,
  `weapon2` int(11) NOT NULL DEFAULT 24,
  `medkit` int(11) NOT NULL DEFAULT 0,
  `skin_1` int(11) NOT NULL DEFAULT 106,
  `skin_2` int(11) NOT NULL DEFAULT 106,
  `skin_3` int(11) NOT NULL DEFAULT 106,
  `skin_4` int(11) NOT NULL DEFAULT 102,
  `skin_5` int(11) NOT NULL DEFAULT 102,
  `skin_6` int(11) NOT NULL DEFAULT 102,
  `skin_7` int(11) NOT NULL DEFAULT 102,
  `skin_8` int(11) NOT NULL DEFAULT 108,
  `skin_9` int(11) NOT NULL DEFAULT 114,
  `skin_10` int(11) NOT NULL DEFAULT 280,
  `model` int(11) NOT NULL DEFAULT 264,
  `hitsound` int(11) NOT NULL DEFAULT 1,
  `hitmarker` int(11) NOT NULL DEFAULT 1,
  `signature_bg` varchar(60) COLLATE utf8mb4_unicode_ci NOT NULL DEFAULT 'loadsc4:loadsc4',
  `signature_av` varchar(60) COLLATE utf8mb4_unicode_ci NOT NULL DEFAULT 'LD_TATT:9gun2',
  `signature_moto` varchar(60) COLLATE utf8mb4_unicode_ci NOT NULL DEFAULT 'Nice one!',
  `signature_bgcolor` int(11) NOT NULL DEFAULT -1,
  `signature_avcolor` int(11) NOT NULL DEFAULT -1,
  `signature_motocolor` int(11) NOT NULL DEFAULT -1,
  `cop_skin` int(8) NOT NULL DEFAULT 280,
  `cop_car` int(8) NOT NULL DEFAULT 596,
  `fug_car` int(8) NOT NULL DEFAULT 566,
  `muted` int(4) NOT NULL DEFAULT 0,
  `muteTime` int(8) NOT NULL DEFAULT 0,
  `activity_1` int(8) NOT NULL DEFAULT 0,
  `activity_2` int(8) NOT NULL DEFAULT 0,
  `activity_3` int(8) NOT NULL DEFAULT 0,
  `perk_1` int(8) NOT NULL DEFAULT 0,
  `hours_1` int(8) NOT NULL DEFAULT 0,
  `hours_2` int(8) NOT NULL DEFAULT 0,
  `hours_3` int(8) NOT NULL DEFAULT 0,
  `donator` int(8) NOT NULL DEFAULT 0,
  `donator_expire` int(12) NOT NULL DEFAULT 0,
  `donator_point` int(8) NOT NULL DEFAULT 0,
  `donator_tag` varchar(64) COLLATE utf8mb4_unicode_ci NOT NULL,
  `perk_2` int(8) NOT NULL DEFAULT 0,
  `perk_3` int(8) NOT NULL DEFAULT 0
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

-- --------------------------------------------------------

--
-- Table structure for table `properties`
--

CREATE TABLE `properties` (
  `id` int(11) NOT NULL,
  `name` varchar(64) CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci NOT NULL DEFAULT 'Unset',
  `entranceX` float NOT NULL,
  `entranceY` float NOT NULL,
  `entranceZ` float NOT NULL,
  `exitX` float NOT NULL,
  `exitY` float NOT NULL,
  `exitZ` float NOT NULL,
  `world` int(8) NOT NULL,
  `interior` int(8) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Indexes for dumped tables
--

--
-- Indexes for table `bans`
--
ALTER TABLE `bans`
  ADD PRIMARY KEY (`ban_id`);

--
-- Indexes for table `players`
--
ALTER TABLE `players`
  ADD PRIMARY KEY (`id`),
  ADD UNIQUE KEY `username` (`username`);

--
-- Indexes for table `properties`
--
ALTER TABLE `properties`
  ADD PRIMARY KEY (`id`);

--
-- AUTO_INCREMENT for dumped tables
--

--
-- AUTO_INCREMENT for table `bans`
--
ALTER TABLE `bans`
  MODIFY `ban_id` int(11) NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT for table `players`
--
ALTER TABLE `players`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT for table `properties`
--
ALTER TABLE `properties`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
