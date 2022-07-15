import React from 'react'
import {
  Navigate,
  Outlet,
} from 'react-router-dom';

import { useAuth } from './GlobalAuth';


type Props = {
}

export default function ProtectedRoutes({}: Props) {
  const { isLogged } = useAuth();

  if(!isLogged()) {
    console.log(isLogged());
    return <Navigate to="/" replace/>
  }
  return <Outlet/>
}