{-# LANGUAGE DataKinds         #-}
{-# LANGUAGE LambdaCase        #-}
{-# LANGUAGE OverloadedStrings #-}
{-# LANGUAGE TypeFamilies      #-}
{-# LANGUAGE TypeOperators     #-}

module Api where

import Data.Proxy
import Data.Text

import Database.Persist

import Models

import Servant.API

type Api = "usuario" :> Capture "email" Text  :> Get '[JSON] (Maybe Usuario)
  :<|> "edificio" :> Capture "id" EdificioId :> Get '[JSON] (Maybe Edificio)
  :<|> "edificio" :> Capture "id" EdificioId :> "bloques" :> Get '[JSON] [Bloque]
  :<|> "puesto" :> Capture "id" BloqueId :> Get '[JSON] [Puesto]

api :: Proxy Api
api = Proxy
