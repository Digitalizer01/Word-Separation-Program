orig="Áacudió"

mod=${orig//[A]/a}
mod=${mod//[E]/e}
mod=${mod//[I]/i}
mod=${mod//[O]/o}
mod=${mod//[U]/u}

mod=${mod//[á]/a}
mod=${mod//[é]/e}
mod=${mod//[í]/i}
mod=${mod//[ó]/o}
mod=${mod//[ú]/u}

mod=${mod//[Á]/a}
mod=${mod//[É]/e}
mod=${mod//[Í]/i}
mod=${mod//[Ó]/o}
mod=${mod//[Ú]/u}

mod=${mod//[ä]/a}
mod=${mod//[ë]/e}
mod=${mod//[ï]/i}
mod=${mod//[ö]/o}
mod=${mod//[ü]/u}

mod=${mod//[Ä]/a}
mod=${mod//[Ë]/e}
mod=${mod//[Ï]/i}
mod=${mod//[Ö]/o}
mod=${mod//[Ü]/u}

echo $mod
