{-# LANGUAGE DeriveGeneric              #-}
{-# LANGUAGE EmptyDataDecls             #-}
{-# LANGUAGE FlexibleContexts           #-}
{-# LANGUAGE GADTs                      #-}
{-# LANGUAGE GeneralizedNewtypeDeriving #-}
{-# LANGUAGE MultiParamTypeClasses      #-}
{-# LANGUAGE OverloadedStrings          #-}
{-# LANGUAGE QuasiQuotes                #-}
{-# LANGUAGE TemplateHaskell            #-}
{-# LANGUAGE TypeFamilies               #-}

module Models where

import GHC.Generics

import Data.Aeson
import Data.Text
import Data.Time.Clock

import Database.Persist.TH

share [mkPersist sqlSettings, mkMigrate "migrateAll"] [persistUpperCase|

Edificio
    nombre Text
    numeroBloques Int
    totalEspaciosParqueo Int
    latitud Double
    longitud Double
    deriving Eq Read Show Generic

Bloque
    edificioId EdificioId
    latitud Double
    longitud Double
    puestosTotales Int
    puestosOcupados Int
    puestosVacios Int
    ultimaActualizacion UTCTime
    deriving Eq Read Show Generic

Puesto
    bloqueId BloqueId
    puesto Int
    ocupado Bool
    deriving Eq Read Show Generic

Usuario
    edificioId EdificioId
    email Text
    nombre Text
    placa Text
    UniqueUsuario email
    deriving Eq Read Show Generic
|]

instance ToJSON Usuario where
instance ToJSON Edificio where
instance ToJSON Bloque where
instance ToJSON Puesto where
