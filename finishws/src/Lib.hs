{-# LANGUAGE DataKinds       #-}
{-# LANGUAGE TemplateHaskell #-}
{-# LANGUAGE TypeOperators   #-}
module Lib
    ( startApp
    , app
    ) where

import Control.Concurrent
import Control.Concurrent.STM.TVar
import Control.Monad.IO.Class
import Control.Monad.STM
import Control.Monad.Trans.Reader
import Data.Aeson
import Data.Aeson.TH
import Network.Wai
import Network.Wai.Handler.Warp
import Servant

type API = "ready" :> Get '[JSON] Bool
    :<|> "done" :> Get '[JSON] ()
    :<|> "working" :> Get '[JSON] ()

startApp :: IO ()
startApp = do
    initial  <- atomically $ newTVar False
    run 3002 $ app $ State initial

data State = State { state :: TVar Bool }

nt :: State -> ReaderT State Handler a -> Handler a
nt s x = runReaderT x s

app :: State -> Application
app s = serve api $ hoistServer api (nt s) server

api :: Proxy API
api = Proxy

server :: ServerT API (ReaderT State Handler)
server = ready :<|> done :<|> working
    where
        ready = do
            State auth <- ask
            liftIO $ atomically $ readTVar auth

        done = do
            State auth <- ask
            liftIO $ atomically $ writeTVar auth True
            return ()
        working = do
            State auth <- ask
            liftIO $ atomically $ writeTVar auth False
            return ()
