-- MySQL dump 10.13  Distrib 8.0.11, for Win64 (x86_64)
--
-- Host: localhost    Database: stoneage
-- ------------------------------------------------------
-- Server version	5.5.5-10.1.32-MariaDB

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
 SET NAMES utf8mb4 ;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `ability`
--

DROP TABLE IF EXISTS `ability`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
 SET character_set_client = utf8mb4 ;
CREATE TABLE `ability` (
  `abi_id` int(11) NOT NULL,
  `class` varchar(8) CHARACTER SET latin1 DEFAULT NULL,
  `statuss` varchar(8) CHARACTER SET latin1 DEFAULT NULL,
  PRIMARY KEY (`abi_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `ability`
--

LOCK TABLES `ability` WRITE;
/*!40000 ALTER TABLE `ability` DISABLE KEYS */;
/*!40000 ALTER TABLE `ability` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `character_info`
--

DROP TABLE IF EXISTS `character_info`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
 SET character_set_client = utf8mb4 ;
CREATE TABLE `character_info` (
  `user_id` varchar(32) COLLATE utf8_unicode_ci NOT NULL,
  `mf` varchar(2) COLLATE utf8_unicode_ci NOT NULL DEFAULT 'M',
  `level` int(11) NOT NULL DEFAULT '1',
  `vital` int(11) NOT NULL DEFAULT '0',
  `str` int(11) NOT NULL DEFAULT '0',
  `tgh` int(11) NOT NULL DEFAULT '0',
  `dex` int(11) NOT NULL DEFAULT '0',
  `attr` int(11) NOT NULL DEFAULT '0',
  `state_pnt` int(11) NOT NULL DEFAULT '0',
  `exp` float(16,2) NOT NULL DEFAULT '0.00',
  `inven_id` varchar(40) COLLATE utf8_unicode_ci NOT NULL,
  `pet_id` varchar(40) COLLATE utf8_unicode_ci NOT NULL,
  `field_location` varchar(60) COLLATE utf8_unicode_ci NOT NULL,
  `nickname` varchar(32) COLLATE utf8_unicode_ci NOT NULL,
  PRIMARY KEY (`user_id`),
  CONSTRAINT `character_info_ibfk_1` FOREIGN KEY (`user_id`) REFERENCES `user_info` (`user_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `character_info`
--

LOCK TABLES `character_info` WRITE;
/*!40000 ALTER TABLE `character_info` DISABLE KEYS */;
INSERT INTO `character_info` VALUES ('admin','M',1,100,1,1,1,1,1,1.00,'In_admin','Pet_admin','','탱커'),('pda','F',1,1,1,11,1,1,1,1.00,'1','1','','1');
/*!40000 ALTER TABLE `character_info` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `inventory`
--

DROP TABLE IF EXISTS `inventory`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
 SET character_set_client = utf8mb4 ;
CREATE TABLE `inventory` (
  `user_id` varchar(40) CHARACTER SET latin1 NOT NULL,
  `item_id` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `inventory`
--

LOCK TABLES `inventory` WRITE;
/*!40000 ALTER TABLE `inventory` DISABLE KEYS */;
/*!40000 ALTER TABLE `inventory` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `item_base`
--

DROP TABLE IF EXISTS `item_base`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
 SET character_set_client = utf8mb4 ;
CREATE TABLE `item_base` (
  `item_id` int(11) NOT NULL,
  `item_name` varchar(32) CHARACTER SET latin1 NOT NULL,
  `str` int(11) DEFAULT '0',
  `tgh` int(11) DEFAULT '0',
  `dex` int(11) DEFAULT '0',
  `vital` int(11) DEFAULT '0',
  `attr` int(11) DEFAULT '0',
  PRIMARY KEY (`item_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `item_base`
--

LOCK TABLES `item_base` WRITE;
/*!40000 ALTER TABLE `item_base` DISABLE KEYS */;
INSERT INTO `item_base` VALUES (1000,'sword',30,0,0,0,0),(2000,'shield',0,15,0,0,0),(3000,'armor',0,0,15,0,0),(4000,'ring',0,0,0,15,0),(5000,'necklace',0,0,0,0,15);
/*!40000 ALTER TABLE `item_base` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `monster_base`
--

DROP TABLE IF EXISTS `monster_base`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
 SET character_set_client = utf8mb4 ;
CREATE TABLE `monster_base` (
  `monster_id` int(11) NOT NULL,
  `monster_name` varchar(32) CHARACTER SET latin1 NOT NULL,
  `type` varchar(16) CHARACTER SET latin1 NOT NULL,
  `class` varchar(16) CHARACTER SET latin1 NOT NULL,
  PRIMARY KEY (`monster_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `monster_base`
--

LOCK TABLES `monster_base` WRITE;
/*!40000 ALTER TABLE `monster_base` DISABLE KEYS */;
/*!40000 ALTER TABLE `monster_base` ENABLE KEYS */;
UNLOCK TABLES;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`root`@`localhost`*/ /*!50003 TRIGGER `stoneage`.`monster_base_BEFORE_DELETE` BEFORE DELETE ON `monster_base` FOR EACH ROW
BEGIN
	delete from `stoneage`.`monster_stat` where `monster_base`.`monster_id` = `monster_stat`.`monster_id`;
END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;

--
-- Table structure for table `monster_stat`
--

DROP TABLE IF EXISTS `monster_stat`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
 SET character_set_client = utf8mb4 ;
CREATE TABLE `monster_stat` (
  `monster_id` int(11) NOT NULL,
  `status_pnt` int(11) NOT NULL,
  `vital` int(11) NOT NULL,
  `str` int(11) NOT NULL,
  `tgh` int(11) NOT NULL,
  `dex` int(11) NOT NULL,
  PRIMARY KEY (`monster_id`),
  CONSTRAINT `monster_id` FOREIGN KEY (`monster_id`) REFERENCES `monster_base` (`monster_id`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `monster_stat`
--

LOCK TABLES `monster_stat` WRITE;
/*!40000 ALTER TABLE `monster_stat` DISABLE KEYS */;
/*!40000 ALTER TABLE `monster_stat` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `status`
--

DROP TABLE IF EXISTS `status`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
 SET character_set_client = utf8mb4 ;
CREATE TABLE `status` (
  `state_id` int(11) NOT NULL,
  `status` varchar(8) CHARACTER SET latin1 DEFAULT NULL,
  `weight` float(10,2) DEFAULT NULL,
  PRIMARY KEY (`state_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `status`
--

LOCK TABLES `status` WRITE;
/*!40000 ALTER TABLE `status` DISABLE KEYS */;
INSERT INTO `status` VALUES (1,'stamina',3.00),(2,'power',1.50),(3,'health',1.00),(4,'elabo',1.00);
/*!40000 ALTER TABLE `status` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `user_info`
--

DROP TABLE IF EXISTS `user_info`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
 SET character_set_client = utf8mb4 ;
CREATE TABLE `user_info` (
  `user_id` varchar(32) COLLATE utf8_unicode_ci NOT NULL,
  `password` varchar(224) COLLATE utf8_unicode_ci NOT NULL,
  PRIMARY KEY (`user_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `user_info`
--

LOCK TABLES `user_info` WRITE;
/*!40000 ALTER TABLE `user_info` DISABLE KEYS */;
INSERT INTO `user_info` VALUES ('admin','d033e22ae348aeb5660fc2140aec35850c4da997'),('pda','77e17e47a656d01098d50696df11219475d66ee6');
/*!40000 ALTER TABLE `user_info` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2018-06-18 18:06:19
