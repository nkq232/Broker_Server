-- phpMyAdmin SQL Dump
-- version 5.1.1
-- https://www.phpmyadmin.net/
--
-- Máy chủ: localhost
-- Thời gian đã tạo: Th10 23, 2021 lúc 06:35 AM
-- Phiên bản máy phục vụ: 10.4.21-MariaDB-log
-- Phiên bản PHP: 8.0.12

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
START TRANSACTION;
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Cơ sở dữ liệu: `Weather`
--

-- --------------------------------------------------------

--
-- Cấu trúc bảng cho bảng `Data`
--

CREATE TABLE `Data` (
  `DataId` int(11) NOT NULL,
  `LocationId` int(11) NOT NULL,
  `TypeId` int(11) NOT NULL,
  `Value` decimal(10,0) NOT NULL,
  `Time` timestamp NOT NULL DEFAULT current_timestamp()
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- --------------------------------------------------------

--
-- Cấu trúc bảng cho bảng `Location`
--

CREATE TABLE `Location` (
  `LocationId` int(11) NOT NULL,
  `LocationName` varchar(50) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

--
-- Đang đổ dữ liệu cho bảng `Location`
--

INSERT INTO `Location` (`LocationId`, `LocationName`) VALUES
(1, 'An Giang'),
(2, 'Vũng Tàu'),
(3, 'Bắc Giang'),
(4, 'Bắc Kạn'),
(5, 'Bạc Liêu'),
(6, 'Bắc Ninh'),
(7, 'Bến Tre'),
(8, 'Bình Định'),
(9, 'Bình Dương'),
(10, 'Bình Phước'),
(11, 'Bình Thuận'),
(12, 'Cà Mau'),
(13, 'Cần Thơ'),
(14, 'Cao Bằng'),
(15, 'Đà Nẵng'),
(16, 'Đắk Lắk'),
(17, 'Đắk Nông'),
(18, 'Điện Biên'),
(19, 'Đồng Nai'),
(20, 'Đồng Tháp'),
(21, 'Gia Lai'),
(22, 'Hà Giang'),
(23, 'Hà Nam'),
(24, 'Hà Nội'),
(25, 'Hà Tĩnh'),
(26, 'Hải Dương'),
(27, 'Hải Phòng'),
(28, 'Hậu Giang'),
(29, 'Hòa Bình'),
(30, 'Hưng Yên'),
(31, 'Khánh Hòa'),
(32, 'Kiên Giang'),
(33, 'Kon Tum'),
(34, 'Lai Châu'),
(35, 'Lâm Đồng'),
(36, 'Lạng Sơn'),
(37, 'Lào Cai'),
(38, 'Long An'),
(39, 'Nam Định'),
(40, 'Nghệ An'),
(41, 'Ninh Bình'),
(42, 'Ninh Thuận'),
(43, 'Phú Thọ'),
(44, 'Phú Yên'),
(45, 'Quảng Bình'),
(46, 'Quảng Nam'),
(47, 'Quảng Ngãi'),
(48, 'Quảng Ninh'),
(49, 'Quảng Trị'),
(50, 'Sóc Trăng'),
(51, 'Sơn La'),
(52, 'Tây Ninh'),
(53, 'Thái Bình'),
(54, 'Thái Nguyên'),
(55, 'Thanh Hóa'),
(56, 'Thừa Thiên Huế'),
(57, 'Tiền Giang'),
(58, 'Thành phố Hồ Chí Minh'),
(59, 'Trà Vinh'),
(60, 'Tuyên Quang'),
(61, 'Vĩnh Long'),
(62, 'Vĩnh Phúc'),
(63, 'Yên Bái');

-- --------------------------------------------------------

--
-- Cấu trúc bảng cho bảng `Register`
--

CREATE TABLE `Register` (
  `RegisterId` int(11) NOT NULL,
  `UserId` int(11) NOT NULL,
  `LocationId` int(11) DEFAULT NULL,
  `TypeId` int(11) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

--
-- Đang đổ dữ liệu cho bảng `Register`
--

INSERT INTO `Register` (`RegisterId`, `UserId`, `LocationId`, `TypeId`) VALUES
(1, 1, 11, 2),
(2, 1, 11, 3),
(3, 1, 15, 4),
(4, 1, 22, 3),
(5, 1, 33, 4),
(6, 2, 12, 3),
(7, 2, 14, 4),
(8, 3, 25, 5),
(9, 3, 22, 6),
(10, 3, 25, 5),
(11, 3, 31, 5),
(12, 4, 26, 6),
(13, 4, 12, 2),
(14, 3, 16, 3),
(15, 2, 17, 4),
(16, 1, 21, 3);

-- --------------------------------------------------------

--
-- Cấu trúc bảng cho bảng `Type`
--

CREATE TABLE `Type` (
  `TypeId` int(11) NOT NULL,
  `TypeName` varchar(50) DEFAULT NULL,
  `TypeValue` varchar(50) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

--
-- Đang đổ dữ liệu cho bảng `Type`
--

INSERT INTO `Type` (`TypeId`, `TypeName`, `TypeValue`) VALUES
(1, 'Temperature', '°C'),
(2, 'Humidity', '%'),
(3, 'Amount of rain', 'mm/12h'),
(4, 'Foresight', 'km'),
(5, 'Wind', 'km/h'),
(6, 'Cloud cover', 'Okta'),
(7, 'Possibility Of Rain', '%'),
(8, 'Lightning', '%');

-- --------------------------------------------------------

--
-- Cấu trúc bảng cho bảng `User`
--

CREATE TABLE `User` (
  `UserId` int(11) NOT NULL,
  `UserName` varchar(50) DEFAULT NULL,
  `Password` varchar(50) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

--
-- Đang đổ dữ liệu cho bảng `User`
--

INSERT INTO `User` (`UserId`, `UserName`, `Password`) VALUES
(1, 'Thach', 'Thach'),
(2, 'Quoc', 'Quoc'),
(3, 'Tuan', 'Tuan'),
(4, 'Quan', 'Quan');

--
-- Chỉ mục cho các bảng đã đổ
--

--
-- Chỉ mục cho bảng `Data`
--
ALTER TABLE `Data`
  ADD PRIMARY KEY (`DataId`),
  ADD KEY `LocationId` (`LocationId`),
  ADD KEY `TypeId` (`TypeId`);

--
-- Chỉ mục cho bảng `Location`
--
ALTER TABLE `Location`
  ADD PRIMARY KEY (`LocationId`);

--
-- Chỉ mục cho bảng `Register`
--
ALTER TABLE `Register`
  ADD PRIMARY KEY (`RegisterId`),
  ADD KEY `UserId` (`UserId`),
  ADD KEY `LocationId` (`LocationId`),
  ADD KEY `TypeId` (`TypeId`);

--
-- Chỉ mục cho bảng `Type`
--
ALTER TABLE `Type`
  ADD PRIMARY KEY (`TypeId`);

--
-- Chỉ mục cho bảng `User`
--
ALTER TABLE `User`
  ADD PRIMARY KEY (`UserId`);

--
-- AUTO_INCREMENT cho các bảng đã đổ
--

--
-- AUTO_INCREMENT cho bảng `Data`
--
ALTER TABLE `Data`
  MODIFY `DataId` int(11) NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT cho bảng `Location`
--
ALTER TABLE `Location`
  MODIFY `LocationId` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=64;

--
-- AUTO_INCREMENT cho bảng `Register`
--
ALTER TABLE `Register`
  MODIFY `RegisterId` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=17;

--
-- AUTO_INCREMENT cho bảng `Type`
--
ALTER TABLE `Type`
  MODIFY `TypeId` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=9;

--
-- AUTO_INCREMENT cho bảng `User`
--
ALTER TABLE `User`
  MODIFY `UserId` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=5;

--
-- Các ràng buộc cho các bảng đã đổ
--

--
-- Các ràng buộc cho bảng `Data`
--
ALTER TABLE `Data`
  ADD CONSTRAINT `Data_ibfk_1` FOREIGN KEY (`LocationId`) REFERENCES `Location` (`LocationId`) ON DELETE CASCADE ON UPDATE CASCADE,
  ADD CONSTRAINT `Data_ibfk_2` FOREIGN KEY (`TypeId`) REFERENCES `Type` (`TypeId`) ON DELETE CASCADE ON UPDATE CASCADE;

--
-- Các ràng buộc cho bảng `Register`
--
ALTER TABLE `Register`
  ADD CONSTRAINT `Register_ibfk_1` FOREIGN KEY (`UserId`) REFERENCES `User` (`UserId`) ON DELETE CASCADE ON UPDATE CASCADE,
  ADD CONSTRAINT `Register_ibfk_2` FOREIGN KEY (`LocationId`) REFERENCES `Location` (`LocationId`) ON DELETE CASCADE ON UPDATE CASCADE,
  ADD CONSTRAINT `Register_ibfk_3` FOREIGN KEY (`TypeId`) REFERENCES `Type` (`TypeId`) ON DELETE CASCADE ON UPDATE CASCADE;
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
